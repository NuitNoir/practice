open($*[0]).each do |title|
  title.strip!
  next if title =~ /[\+\-\.\$\(\)\?\*!"'_,]+/

    title_length = title.split(//u).length
  score = [-36000.0, -400 * (title_length ** 1.5)].max.to_i

  if title_length > 3
    print"#{title},0,0,#{score},名詞,固有名詞,*,*,*,*,#{title},*,*,wikipedia,\n"
  end
end
