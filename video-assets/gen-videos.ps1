[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$ErrorActionPreference = "Continue"

$cfg = Get-Content "C:\Users\36025\.config\opencode\opencode.json" -Raw | ConvertFrom-Json
$key = $cfg.provider.agnes.options.apiKey
Write-Output ("密钥: " + $key.Substring(0,8) + "****")
$dest = Join-Path ([Environment]::GetFolderPath("Desktop")) "创作"
Write-Output ("输出目录: " + $dest)

$neg = "模糊,变形,乱码文字,低质量,抖动剧烈"
$jobs = @(
  @{ name = "01_ai_intro.mp4"; prompt = "电影感科技开场镜头:一个发光的人类大脑悬浮在深蓝色科技空间中央,脑内神经元与金色电路交织生长,光芒逐渐扩散,数据粒子环绕飞舞,镜头缓慢推进,赛博朋克与极简科技风格,深蓝与金色配色,高细节,运镜流畅平稳" },
  @{ name = "02_llm.mp4"; prompt = "动画特效:一条流淌的光河由无数发光汉字字符组成,从四面八方汇聚并压缩进一个悬浮的发光水晶大脑,大脑越来越亮,随后大脑弹出一串发光字符接龙依次点亮,粒子文字特效,深色背景,蓝紫色调,镜头缓慢环绕,科技感十足" },
  @{ name = "03_agent.mp4"; prompt = "动画:一个发光的大脑被透明玻璃罩罩住悬浮在科技感桌面场景中,玻璃罩出现裂纹后碎裂,碎片飞散,两条精密机械手臂从大脑两侧伸出,在发光键盘上快速敲击,旁边屏幕亮起并出现日历界面打上金色对勾,蓝金配色,镜头缓慢推进" },
  @{ name = "04_distill.mp4"; prompt = "动画:左侧一个巨大的发光大脑模型,通过一根发光管道向下方的迷你芯片注入金色光液,迷你芯片吸收光芒后逐渐点亮,一大一小体积对比悬殊,化学蒸馏装置意象与科技元素融合,深色实验室背景,金色与青色光效,粒子飞溅,镜头从左向右缓慢平移" },
  @{ name = "05_ending.mp4"; prompt = "结尾镜头:一部智能手机悬浮在浩瀚星空中央,屏幕亮起,一道金色光柱从屏幕升起化作漫天星点,星点逐渐连接成神经网络星座图案,镜头缓缓拉远,宁静而史诗,深蓝与金色配色,电影质感" }
)

function Get-VideoUrl($d) {
  if ($d.url) { return $d.url }
  if ($d.metadata -and $d.metadata.url) { return $d.metadata.url }
  if ($d.data) {
    if ($d.data.url) { return $d.data.url }
    if ($d.data.metadata -and $d.data.metadata.url) { return $d.data.metadata.url }
  }
  return $null
}

$tasks = @()
foreach ($j in $jobs) {
  $bodyObj = @{
    model           = "agnes-video-v2.0"
    prompt          = $j.prompt
    width           = 1280
    height          = 720
    num_frames      = 121
    frame_rate      = 24
    negative_prompt = $neg
  }
  $json = ConvertTo-Json -InputObject $bodyObj -Depth 5
  $bytes = [System.Text.Encoding]::UTF8.GetBytes($json)
  try {
    $r = Invoke-WebRequest -Uri "https://apihub.agnes-ai.com/v1/videos" -Method POST -Headers @{ Authorization = "Bearer $key" } -ContentType "application/json; charset=utf-8" -Body $bytes -UseBasicParsing -TimeoutSec 120
    $d = $r.Content | ConvertFrom-Json
    $vid = $d.video_id
    if (-not $vid) { $vid = $d.id }
    if (-not $vid) { throw "响应中缺少 video_id" }
    $tasks += @{ name = $j.name; id = $vid }
    Write-Output ("SUBMITTED: " + $j.name + " id=" + $vid)
  }
  catch {
    Write-Output ("SUBMIT FAIL: " + $j.name + " -> " + $_.Exception.Message)
  }
}

$deadline = (Get-Date).AddMinutes(25)
$pending = @($tasks)
while ($pending.Count -gt 0 -and (Get-Date) -lt $deadline) {
  Start-Sleep -Seconds 15
  $still = @()
  foreach ($t in $pending) {
    try {
      $r = Invoke-WebRequest -Uri ("https://apihub.agnes-ai.com/v1/agnesapi?video_id=" + [uri]::EscapeDataString($t.id)) -Headers @{ Authorization = "Bearer $key" } -UseBasicParsing -TimeoutSec 60
      $d = $r.Content | ConvertFrom-Json
      $status = $d.status
      if (-not $status -and $d.data) { $status = $d.data.status }
      if ($status -eq "completed") {
        $url = Get-VideoUrl $d
        if ($url) {
          $outFile = Join-Path $dest $t.name
          Invoke-WebRequest -Uri $url -OutFile $outFile -UseBasicParsing -TimeoutSec 600
          $len = (Get-Item $outFile).Length
          Write-Output ("OK: " + $t.name + " (" + [math]::Round($len/1MB, 1) + " MB)")
        }
        else {
          Write-Output ("FAIL(无url): " + $t.name)
        }
      }
      elseif ($status -eq "failed") {
        Write-Output ("FAIL: " + $t.name + " -> " + ($d | ConvertTo-Json -Depth 3))
      }
      else {
        $still += $t
      }
    }
    catch {
      $still += $t
      Write-Output ("POLL ERR: " + $t.name + " -> " + $_.Exception.Message)
    }
  }
  $pending = $still
  Write-Output ("进行中: " + $pending.Count + "/5")
}

if ($pending.Count -gt 0) {
  Write-Output ("超时未完成: " + (($pending | ForEach-Object { $_.name }) -join ", "))
}
Write-Output "ALL DONE"
