require 'date'
class Reservation
  attr_reader :id
  attr_accessor :start_at, :end_at, :tables
  def initialize(hash)
    p hash
    @id = hash[:id]
    @start_at = DateTime.parse(hash[:start_at])
    @end_at = DateTime.parse(hash[:end_at])
    @tables = hash[:tables]
  end

  def detect_over_booking(reservations)
    over_books = []
    reservations.each do |reservation|
      next if self.id == reservation.id
      next unless is_same_table?(reservation.tables)
      next unless is_time_conflict?(self.start_at, self.end_at, reservation.start_at, reservation.end_at)
      over_books.push(reservation.id)      
    end
    over_books
  end

  def is_same_table?(tables)
    self.tables.inject(false) {
      |flag, table_num| flag = true if tables.include?(table_num) }
  end

  def is_time_conflict?(start1, end1, start2, end2)
    flag = false
    #flag = true if start1.between?(start2, end2) # 境界含んでしまうのでダメ
    #flag = true if start2.between?(start1, end1) # 境界含んでしまうのでダメ
    flag = true if start1 > start2 and start1 < end2
    flag = true if start2 > start1 and start2 < end1
    flag
  end
end

def main
  reservations = get_reservations
  books = []
  reservations.each_with_index do |reservation, i|
    books[i] = []
    books[i].push(reservation.id)
    over_books = reservation.detect_over_booking(reservations)
    books[i].push(over_books)
  end
  p books
end

def get_reservations
  reservations = []
  reservation_array = get_reservation_array
  reservation_array.each do |reservation_hash|
    reservations.push(Reservation.new(reservation_hash))
  end
  reservations
end

def get_reservation_array
  [
    {
    "id": 1,
    "start_at": "2015-07-13 18:00:00 +0900",
    "end_at": "2015-07-13 19:30:00 +0900",
    "tables": [
      "A"
  ]
  },
    {
    "id": 2,
    "start_at": "2015-07-13 18:30:00 +0900",
    "end_at": "2015-07-13 20:00:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 3,
    "start_at": "2015-07-13 19:30:00 +0900",
    "end_at": "2015-07-13 21:00:00 +0900",
    "tables": [
      "C"
  ]
  },
    {
    "id": 4,
    "start_at": "2015-07-13 19:30:00 +0900",
    "end_at": "2015-07-13 22:00:00 +0900",
    "tables": [
      "D",
      "E"
  ]
  },
    {
    "id": 5,
    "start_at": "2015-07-14 18:00:00 +0900",
    "end_at": "2015-07-14 19:00:00 +0900",
    "tables": [
      "A"
  ]
  },
    {
    "id": 6,
    "start_at": "2015-07-14 19:00:00 +0900",
    "end_at": "2015-07-14 20:30:00 +0900",
    "tables": [
      "A"
  ]
  },
    {
    "id": 7,
    "start_at": "2015-07-14 20:30:00 +0900",
    "end_at": "2015-07-14 21:45:00 +0900",
    "tables": [
      "A"
  ]
  },
    {
    "id": 8,
    "start_at": "2015-07-14 18:30:00 +0900",
    "end_at": "2015-07-14 20:00:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 9,
    "start_at": "2015-07-14 19:45:00 +0900",
    "end_at": "2015-07-14 21:15:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 10,
    "start_at": "2015-07-14 18:30:00 +0900",
    "end_at": "2015-07-14 20:00:00 +0900",
    "tables": [
      "C",
      "D",
      "E"
  ]
  },
    {
    "id": 11,
    "start_at": "2015-07-15 18:00:00 +0900",
    "end_at": "2015-07-15 19:30:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 12,
    "start_at": "2015-07-15 18:00:00 +0900",
    "end_at": "2015-07-15 19:30:00 +0900",
    "tables": [
      "C"
  ]
  },
    {
    "id": 13,
    "start_at": "2015-07-15 18:00:00 +0900",
    "end_at": "2015-07-15 19:30:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 14,
    "start_at": "2015-07-15 19:15:00 +0900",
    "end_at": "2015-07-15 20:45:00 +0900",
    "tables": [
      "C",
      "D",
      "E"
  ]
  },
    {
    "id": 15,
    "start_at": "2015-07-15 20:45:00 +0900",
    "end_at": "2015-07-15 22:15:00 +0900",
    "tables": [
      "A",
      "C",
      "B",
      "D",
      "E"
  ]
  },
    {
    "id": 16,
    "start_at": "2015-07-14 21:00:00 +0900",
    "end_at": "2015-07-14 22:30:00 +0900",
    "tables": [
      "B"
  ]
  },
    {
    "id": 17,
    "start_at": "2015-07-14 19:00:00 +0900",
    "end_at": "2015-07-14 20:30:00 +0900",
    "tables": [
      "E"
  ]
  }
  ]
end

main()


