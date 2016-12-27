/**
 * This file is a part of YAMFD, see the license.txt file for copyright details. 
 **/
#ifndef __SMC_H__
#define __SMC_H__

/**
 * @brief initialize the Apple SMC wrapper
 * @return status code
 **/
int smc_init();

/**
 * @brief finalize the Apple SMC wrapper
 * @return status code
 **/
int smc_finalize();

/**
 * @brief get the number of sensors
 * @return the number of sensors or status code
 **/
int smc_sensor_get_count();

/**
 * @brief Get the sensor label by its ID
 * @param id the sensor ID
 * @return the label, NULL if there's error
 **/
const char* smc_sensor_get_label(int id);

/**
 * @brief Get the temperature from the sensor 
 * @param id the sensor ID
 * @return the temperature in 0.001 celisus or negative error code
 **/
int smc_sensor_get_temp(int id);

/**
 * @brief Get the number of fans the system have
 * @return the number of fans or negative error code
 **/
int smc_fan_get_count();

/**
 * @brief Get current fan speed
 * @param id the sensor ID
 * @return the speed in RPM or negative error code
 **/
int smc_fan_get_speed(int id);

/**
 * @brief Set fan speed
 * @param id the sensor ID
 * @param speed the new speed
 * @return status code (negative on error case)
 **/
int smc_fan_set_speed(int id, int speed);

#endif /* __SMC_H__ */
