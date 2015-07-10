require 'csv'
require 'mechanize'
require 'nokogiri'

class Crawler
  def initialize(base_url,sub_url, file_name, page_path, next_path, info_pathes)
    @agent = Mechanize.new
    @sub_url = sub_url
    @base_url = base_url
    @page_path = page_path
    @next_path = next_path
    @info_pathes = info_pathes
    @csv_path = "#{Rails.root}/script/crawler/csv/"
    @csv_file = @csv_path + file_name
    CSV.open(@csv_file, 'w') do |file|
    end
  end
  def crawl
    array = []
    page = nil
    while page = @agent.get(@base_url + @sub_url)
      page_nodes = page.search(@page_path)
      page_nodes.each_with_index do |node, i|
        array[i] = []
        p name = node.inner_text.gsub(/( |\n)/, '')
        array[i].push(name)
        for num in 1..5 do
          p @page_url = @base_url + node['href'] + "&func=#{num}"
          #URL末尾をfunc=2
          if @info_pathes != []
            detail_page = @agent.get(@page_url)
            @info_pathes.each do |info_path|
              @infos = detail_page.search(info_path)
              @infos.each do |info|
                lines = info.text.delete("\r").split("\n")
                lines.each do |line|
                  line = line.gsub(/ /, "")
                  if line.include?("TEL") || line.include?("@") || line.include?("電話") || line.include?("問い合") || line.include?("＠") || line.include?("ダイアル")
                    array[i].push(line)
                  end
                end
              end
            end
          end
        end
      end
      array.each do |infos|
        CSV.open(@csv_file, 'a') do |writer|
          p infos
          writer << infos
        end
      end
      p array
      p @next_url = page.search(@next_path).first
      if @next_url
        @next_url = @next_url['href']
      else
        return
      end
      @sub_url = @next_url
    end
  end
end

base_url = 'https://acaric.jp/modules/acaricuserpub/'
sub_url = '?action=searchCompany&perpage=15&start=0'
file_name = 'akariku_corporations.csv'
page_path = '#contents > div > div > h2 > a'
next_path = 'ul.pager > li.next > a'
info_pathes = ['table.data-table > td']
#p04 > table:nth-child(18) > tbody > tr > td
crawler = Crawler.new(base_url, sub_url, file_name, page_path, next_path, info_pathes)
crawler.crawl

