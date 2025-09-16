param(
  [int]$Port = 5500,
  [string]$Root = "..\html"
)

$ErrorActionPreference = 'Stop'

function Write-Info($msg) { Write-Host "[INFO] $msg" -ForegroundColor Cyan }
function Write-Warn($msg) { Write-Host "[WARN] $msg" -ForegroundColor Yellow }
function Write-Err($msg)  { Write-Host "[ERR ] $msg" -ForegroundColor Red }

# Resolve root path
$Root = [System.IO.Path]::GetFullPath((Join-Path -Path $PSScriptRoot -ChildPath $Root))
if (-not (Test-Path $Root)) { Write-Err "根目录不存在: $Root"; exit 1 }

# Build content type map
$contentTypes = @{ 
  '.html'='text/html; charset=utf-8'; '.htm'='text/html; charset=utf-8';
  '.js'='text/javascript; charset=utf-8'; '.mjs'='text/javascript; charset=utf-8';
  '.css'='text/css; charset=utf-8';
  '.json'='application/json; charset=utf-8'; '.map'='application/json; charset=utf-8';
  '.svg'='image/svg+xml; charset=utf-8'; '.xml'='application/xml; charset=utf-8';
  '.png'='image/png'; '.jpg'='image/jpeg'; '.jpeg'='image/jpeg'; '.gif'='image/gif'; '.webp'='image/webp'; '.ico'='image/x-icon';
  '.mp4'='video/mp4'; '.webm'='video/webm'; '.ogg'='video/ogg';
  '.mp3'='audio/mpeg'; '.wav'='audio/wav'; '.m4a'='audio/mp4';
  '.txt'='text/plain; charset=utf-8'; '.csv'='text/csv; charset=utf-8';
  '.pdf'='application/pdf'
}

Add-Type -AssemblyName System.Net.HttpListener
$listener = [System.Net.HttpListener]::new()
$prefix = "http://127.0.0.1:$Port/"
$listener.Prefixes.Add($prefix)

try {
  $listener.Start()
} catch {
  Write-Err "无法启动监听端口 $Port。请尝试以管理员身份运行或更换端口。"; exit 1
}

Write-Info "静态服务器已启动：$prefix"
Write-Info "根目录：$Root"
Write-Info "按 Ctrl+C 终止。"

$cancelled = $false
$null = Register-EngineEvent PowerShell.Exiting -Action { $script:cancelled = $true }

function Get-LocalPath([string]$urlPath) {
  if ([string]::IsNullOrWhiteSpace($urlPath)) { return Join-Path $Root 'index.html' }
  $decoded = [System.Uri]::UnescapeDataString($urlPath.TrimStart('/'))
  if ($decoded.EndsWith('/')) { $decoded = $decoded + 'index.html' }
  $safe = $decoded -replace "[\\/]+","/"
  $combined = Join-Path $Root $safe
  $full = [System.IO.Path]::GetFullPath($combined)
  if (-not $full.StartsWith($Root)) { return $null }
  return $full
}

while (-not $cancelled) {
  try {
    $ctx = $listener.GetContext()
    Start-Job -ArgumentList $ctx -ScriptBlock {
      param($ctx)
      try {
        $req = $ctx.Request
        $res = $ctx.Response
        $path = Get-LocalPath $req.Url.AbsolutePath
        if (-not $path) {
          $res.StatusCode = 400; $res.Close(); return
        }
        if (-not (Test-Path $path)) {
          $res.StatusCode = 404
          $msg = [System.Text.Encoding]::UTF8.GetBytes("Not Found")
          $res.OutputStream.Write($msg,0,$msg.Length)
          $res.Close(); return
        }
        $ext = [System.IO.Path]::GetExtension($path).ToLowerInvariant()
        $ctype = $using:contentTypes[$ext]
        if (-not $ctype) { $ctype = 'application/octet-stream' }
        $bytes = [System.IO.File]::ReadAllBytes($path)
        $res.ContentType = $ctype
        $res.ContentLength64 = $bytes.LongLength
        $res.AddHeader('Cache-Control','no-cache, no-store, must-revalidate')
        $res.AddHeader('Pragma','no-cache')
        $res.AddHeader('Expires','0')
        $res.OutputStream.Write($bytes,0,$bytes.Length)
        $res.Close()
      } catch {
        try { $ctx.Response.StatusCode = 500; $ctx.Response.Close() } catch {}
      }
    } | Out-Null
  } catch {
    if (-not $cancelled) { Write-Warn "请求处理异常: $_" }
  }
}

try { $listener.Stop(); $listener.Close() } catch {} 