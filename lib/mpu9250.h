#pragma once
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * I2Cモードで初期化する
 * @param[in] dev_addr デバイスアドレス(0x68 or 0x69)
 * @return 0:成功、0以外:エラー
 */
int mpu9250_i2c_init(int dev_addr);

/**
 * SPIモードで初期化する
 * @param[in] cs チップセレクト(0 or 1)
 * @return 0:成功、0以外:エラー
 */
int mpu9250_spi_init(int cs);

/**
 * 加速度を取得する
 * @param[out] accel 3 * 2バイトのバッファ
 * @return 0:成功、0以外:エラー
 */
int mpu9250_read_accel(short* accel);

/**
 * ジャイロを取得する
 * @param[out] gyro 3 * 2バイトのバッファ
 * @return 0:成功、0以外:エラー
 */
int mpu9250_read_gyro(short* gyro);

/**
 * 方位を取得する(I2Cモードのみ)
 * @param[out] mag 3 * 2バイトのバッファ
 * @return 0:成功、0以外:エラー
 */
int mpu9250_read_mag(short* mag);

#ifdef __cplusplus
}
#endif /* __cplusplus */
