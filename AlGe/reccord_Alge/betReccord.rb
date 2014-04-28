#!/usr/bin/ruby

for dim in 5..10
    for larg in 2..25 
        puts dim.to_s + " " + larg.to_s
        $compileProg = 'make opt CPPFLAG=-DDIM="'+dim.to_s+" -DLARGEUR="+larg.to_s + '"'           

        value = `#{$compileProg}`
        if (!$?.success?)
            puts "error during the execution"
            exit
        else
           # puts value
        end

        execCmd = "./latinSquare"
        value = `#{execCmd}`
        if (!$?.success?)
            puts "error during the execution"
            exit
        else
            puts value

        end
    end
end
