require './load'

exit 1 unless MPU9250.mpu9250_i2c_init(0x68).zero?

puts((%w[accel gyro mag].product %w[x y z]).map { |a, b| format('%8s', a + b) }.join)

loop do
  accel, gyro, mag = Array.new(3) { Array.new(6) { 0 }.pack('C*') }
  exit 1 unless MPU9250.mpu9250_read_accel(accel)
  exit 1 unless MPU9250.mpu9250_read_gyro(gyro)
  exit 1 unless MPU9250.mpu9250_read_mag(mag)
  puts([accel, gyro, mag].map do |d|
    d.unpack('s!*').map { |v| format('%8d', v) }
  end.flatten.join)
  sleep(0.5)
end
