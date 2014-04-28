#!/usr/bin/ruby

$nameProg = "./latinSquare"
$keyWord = "Score:"

usage = "usage: computeStats.rb nbrun[=100] [nameProg] [keyWord]"

if (ARGV.length > 0)
    nbrun = ARGV[0].to_i
    if (nbrun == 0)
        puts usage
    end
else
    nbrun = 100
end

if (ARGV.length > 1)
    $nameProg = ARGV[1].to_s
end

if (ARGV.length > 2)
    $keyWord = ARGV[2].to_s
end

if (ARGV.length > 3)
    puts usage
end

def launchProg()
    cmd = $nameProg
    value = `#{cmd}`
    if (!$?.success?)
        exit
    end
    res = value.scan(/#{$keyWord} (\d*)/)   # res is a list of all the occurence of keyword
    if (res.length == 0)
        puts ""
        puts "keyword \"#{$keyWord}\" not found during the execution of the program"
        exit
    end
    return res
end

def meanArray(list)
        return (list.reduce(:+)).to_f / list.size
end

def sdArray(list)
    m = meanArray(list)
    variance = list.inject(0) { |variance, x| variance += (x - m) ** 2 }
    return Math.sqrt(variance/(list.size-1))
end

def ciArray(list)
    s = sdArray(list)
    return 1.96*s/Math.sqrt(list.size)
end


def printRes(listRes,listTime)
    for i in 0...listRes.length
        puts meanArray(listRes[i]).to_s + " +- " + ciArray(listRes[i]).to_s + " (sd: " + sdArray(listRes[i]).to_s + ") (max: " + listRes[i].max.to_s + ")"
    end
    puts meanArray(listTime).to_s + "s"
end

begin

    listRes = []
    listTime = []
    for i in 1..nbrun
        STDERR.printf("\r%i / %i",i,nbrun )
        STDERR.flush
        startt = Time.now
        res = launchProg()
        if (res.length>listRes.length)
            for j in listRes.length...res.length
                listRes[j]=[]
            end
        end
        for j in 0...res.length
            listRes[j].push(res[j][0].to_i)
        end
        endt = Time.now
        listTime.push(endt-startt)
    end
    STDERR.puts ""
    printRes(listRes,listTime)

rescue Exception => e
    STDERR.puts ""
    if (i>2)
        printRes(listRes,listTime)
    else
        puts "not enough stats"
    end
end

