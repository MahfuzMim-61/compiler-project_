@echo off
rem generate_compact_tokens.bat
rem Usage: generate_compact_tokens.bat <source-file>
if "%~1"=="" (
  echo Usage: %~nx0 ^<source-file^>
  exit /b 1
)

rem Run lexer and capture output to a temp file
set "SRC=%~1"
set "TMP=__lexer_out.tmp"
del /q "%TMP%" 2>nul
.\project_lexer.exe "%SRC%" > "%TMP%" 2>&1

rem Use PowerShell to extract token block and write compact file
powershell -NoProfile -Command ^
  "$out = Get-Content -Raw -LiteralPath '%TMP%';" ^
  "$m = [regex]::Match($out,'==== TOKEN STREAM ====(.*?)==== END TOKEN STREAM ====', 'Singleline');" ^
  "if ($m.Success) { $block = $m.Groups[1].Value.Trim(); $compact = ($block -split '\\s+' ) -join ' '; $safe = [Regex]::Replace('%SRC%','[^A-Za-z0-9]','_'); Set-Content -LiteralPath ('compact_' + $safe + '.txt') -Value $compact -Encoding ascii; Write-Host ('Wrote compact_' + $safe + '.txt') } else { Write-Host 'Token stream markers not found for' '%SRC%'; exit 2 }"

del /q "%TMP%" 2>nul

