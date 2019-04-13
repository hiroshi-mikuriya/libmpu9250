libmpu9250.so
=============

|ディレクトリ|説明|
|:---|:---|
|lib|libmpu9250.soのソースコード|
|ruby|rubyでlibmpu9250を使うサンプル|
|python|pythonでlibmpu9250を使うサンプル|
|go|goでlibmpu9250を使うサンプル|
|cpp|c++でlibmpu9250を使うサンプル|

# インストール手順

```
$ cd lib
$ sudo make install
$ sudo ldconfig
```

# 実行手順

## ruby

```
$ cd ruby
$ ruby i2c.rb # I2Cモード実行
$ ruby spi.rb # SPIモード実行
```

## python

```
$ cd python
$ python i2c.py # I2Cモード実行
$ python spi.py # SPIモード実行
```

## go

```
$ cd go
$ go build
$ ./go
```

## c++

```
$ cd cpp
$ make
$ ./run
```