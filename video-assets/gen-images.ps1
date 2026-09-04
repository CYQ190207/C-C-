[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$ErrorActionPreference = "Continue"

$cfg = Get-Content "C:\Users\36025\.config\opencode\opencode.json" -Raw | ConvertFrom-Json
$key = $cfg.provider.agnes.options.apiKey
Write-Output ("使用密钥: " + $key.Substring(0,8) + "****")

$jobs = @(
  @{ name = "img_00_cover.png"; prompt = "科技感视频封面插画:画面中央一个巨大的发光人类大脑,大脑一半是神经网络一半是金色电路,周围环绕四个发光小图标:靶心、对话气泡、机械手臂、化学烧瓶,深蓝色背景,金色粒子光效,电影级光感,构图大气饱满,画面中不要出现任何文字" },
  @{ name = "img_01_ai.png"; prompt = "科技概念插画:一个巨大的发光伞状穹顶笼罩住下方各种智能机器人——聊天机器人、下棋机器人、工业机械臂、自动驾驶汽车,象征人工智能是包罗万象的宏大目标;画面左侧是怀旧风格的工程师在黑板上写满规则,渐变过渡到右侧机器自主学习的数据流瀑布,深蓝金配色,扁平科技插画风格,构图有故事感,画面中不要出现任何文字" },
  @{ name = "img_02_llm.png"; prompt = "科技概念插画:一位由光影构成的书卷学霸坐在书山之巅,面前悬浮着一条由无数发光汉字组成的光河,光河从四面八方汇入学霸手中的发光水晶球,水晶球内字符正在自动接龙点亮,背景是宏伟的图书馆穹顶与漫天书页飞舞,蓝紫色调,细节丰富,画面中不要出现任何文字" },
  @{ name = "img_03_agent.png"; prompt = "科技概念插画:一个发光的大脑被透明玻璃罩罩住悬浮在科技感桌面上方,玻璃罩正碎裂开来,碎片化作金色粒子飞散,两条精密机械手臂从罩中伸出,一条在发光键盘上敲击,一条在悬浮的日历界面上打金色对勾,周围漂浮着搜索图标与浏览器窗口,深蓝背景,金蓝配色,动感十足,画面中不要出现任何文字" },
  @{ name = "img_04_distill.png"; prompt = "科技概念插画:左侧一个巨大的发光大脑模型,通过发光玻璃管道向右侧一个迷你芯片注入金色光液,迷你芯片吸收光液后逐渐点亮,中间点缀化学蒸馏烧瓶的玻璃器皿意象,金色光液滴落泛起涟漪,一大一小体积对比强烈,深色实验室背景,金色与青色光效,粒子飞溅,画面中不要出现任何文字" }
)

$outDir = "E:\opencode\video-assets"

foreach ($j in $jobs) {
  $outFile = Join-Path $outDir $j.name
  Write-Output ("---- 生成: " + $j.name + " ----")
  try {
    $bodyObj = @{
      model      = "agnes-image-2.1-flash"
      prompt     = $j.prompt
      size       = "2K"
      ratio      = "16:9"
      extra_body = @{ response_format = "url" }
    }
    $json = ConvertTo-Json -InputObject $bodyObj -Depth 5
    $bytes = [System.Text.Encoding]::UTF8.GetBytes($json)
    $r = Invoke-WebRequest -Uri "https://apihub.agnes-ai.com/v1/images/generations" -Method POST -Headers @{ Authorization = "Bearer $key" } -ContentType "application/json; charset=utf-8" -Body $bytes -UseBasicParsing -TimeoutSec 300
    $url = ($r.Content | ConvertFrom-Json).data[0].url
    if (-not $url) { throw "响应中缺少图片 URL" }
    Invoke-WebRequest -Uri $url -OutFile $outFile -UseBasicParsing -TimeoutSec 300
    $len = (Get-Item $outFile).Length
    Write-Output ("OK: " + $j.name + " (" + [math]::Round($len/1KB) + " KB)")
  }
  catch {
    Write-Output ("FAIL: " + $j.name + " -> " + $_.Exception.Message)
  }
}
Write-Output "全部完成"
