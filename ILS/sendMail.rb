#!/usr/bin/ruby
# script used to send mail with ruby
require 'net/smtp'

def usage()
    puts "usage: sendMail.rb \"message\" \"title\""
end

if (ARGV.length > 0)
    $message = ARGV[0].to_s
else
    puts "error: no message selected"
    usage()
    exit
end

if (ARGV.length > 1)
    $title = ARGV[1].to_s
else
    puts "warning: no title selected"
    usage()
    exit
end



message = <<MESSAGE_END
From: Server <server@nodomain.com>
To: Arpad Rimmel <arpad.rimmel@gmail.com>
Subject: #{$title}

#{$message}
MESSAGE_END

smtp = Net::SMTP.new('smtp.gmail.com',587)
smtp.enable_starttls

smtp.start('gmail.com','mail.sender.2245@gmail.com', 'pass2245', :login)
smtp.send_message message, 'mail.sender@gmail.com', 'arpad.rimmel@gmail.com'
smtp.finish
