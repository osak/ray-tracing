Dir.glob('*.png').sort.each do |file|
    puts "## #{file}"
    puts "![](#{file})"
    puts ""
end