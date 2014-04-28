#!/usr/bin/ruby

$nameProg = "./latinSquare"
$keyWord = ["Score:"]
#$keyWord = ["Score:","Eval:"]
$nbSignificantNumbers = 1
$format = "%." + $nbSignificantNumbers.to_s + "f"

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
    $keyWord = ARGV[2].to_s.split
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
    res = {}
    $keyWord.each do |keyword|
        tempRes = value.scan(/#{keyword} (\d*)/)   # res is a list of all the occurence of keyword
        if (tempRes.length == 0)
            puts ""
            puts "keyword \"#{keyword}\" not found during the execution of the program"
            exit
        end
        res.store(keyword,tempRes)   
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
    l1 = "#"
    l2 = "#"
    listRes.keys.each do |key|
        l1 += key.to_s + "\t\t"
        l2 += "mean\t" + "ci\t"
    end
    l1 += "Time"
    puts l1
    puts l2
    nbLines = listRes.values.map{|x| x.length}.max
    for i in 0...nbLines do
        s = ""
        listRes.keys.each do |key|
            if (i < listRes[key].length)
                s += $format % meanArray(listRes[key][i]).to_s + "\t" + $format % ciArray(listRes[key][i]).to_s + "\t" #+ sdArray(listRes[key][i]).to_s + "\t" + listRes[key][i].max.to_s + "\t"
            else
                s += $format % meanArray(listRes[key][-1]).to_s + "\t" + $format % ciArray(listRes[key][-1]).to_s + "\t" # + sdArray(listRes[key][-1]).to_s + "\t" + listRes[key][-1].max.to_s + "\t"
            end
        end
        s += $format % meanArray(listTime).to_s 
        puts s
    end
    #puts meanArray(listTime).to_s + "s"
end

begin

    #init
    listRes = {}
    $keyWord.each do |key|
        listRes.store(key,[])
    end
    listTime = []

    for i in 1..nbrun
        STDERR.printf("\r%i / %i",i,nbrun )
        STDERR.flush
        startt = Time.now
        res = launchProg()

        # if a new results has more values for a certain key, add new rows in listRes
        res.keys.each do |resKey|
            if (res[resKey].length>listRes[resKey].length)
                for j in listRes[resKey].length...res[resKey].length
                    listRes[resKey].push([])
                end
            end
        end

        # add the results to listRes
        listRes.keys.each do |resKey|
            for j in 0...res[resKey].length
                listRes[resKey][j].push(res[resKey][j][0].to_i)
            end
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

