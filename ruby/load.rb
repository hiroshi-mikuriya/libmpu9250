require 'fiddle/import'

module MPU9250
  extend Fiddle::Importer
  dlload 'libmpu9250.so'
  extern 'int mpu9250_i2c_init(int dev_addr)'
  extern 'int mpu9250_spi_init(int cs)'
  extern 'int mpu9250_read_accel(short * accel)'
  extern 'int mpu9250_read_gyro(short * gyro)'
  extern 'int mpu9250_read_mag(short * mag)'
end
