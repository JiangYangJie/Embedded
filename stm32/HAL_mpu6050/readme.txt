MPU_6050_Init();
mpu_dmp_init();
mpu_dmp_get_data(&t3,&t4,&t5);

使用时只需更改mpu6050.c里的i2c即可
