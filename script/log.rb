def failed(ans, res, line)
  if ans.downcase != res.downcase then
    puts "failed: #{ans} != #{res} at #{line}"
    return true
  end
  return false
end

log = File.open("../roms/nestest.log", "r")
nes = File.open("./nemu.txt")

while(1) do
  ans = log.readline.split("  ");
  res = nes.readline.split(" ");
  ans.delete("")
  res.delete("")
  ans_regstate = ans[3].split
  ansa = ans_regstate[0]
  ansx = ans_regstate[1]
  ansy = ans_regstate[2]
  p ans
  p res

  if failed(ans[0], res[0], nes.lineno) then
    break
  end

  if failed(ansa, res[2], nes.lineno) then
    break
  end

  if failed(ansx, res[3], nes.lineno) then
    break
  end

  if failed(ansy, res[4], nes.lineno) then
    break
  end
end
