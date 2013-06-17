#!/usr/bin/ruby

nameIn = "res"
nameOut = "res.dat"

if (ARGV.length > 0)
    nameIn = ARGV[0].to_s
end

if (ARGV.length > 1)
    nameOut = ARGV[1].to_s
end

begin
fileIn = File.new(nameIn,"r")
fileOut = File.new(nameOut,"w")

while((lineX = fileIn.gets) && (lineY = fileIn.gets))
    fileOut.puts(lineX.split(" ")[0] + "\t" + lineY.split(" ")[0] + "\t" + lineY.split(" ")[2])
end
fileIn.close
fileOut.close


rescue => err
    puts err.to_s
end
