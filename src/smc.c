#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <smc.h>
#define _HWMON_PATH "/sys/class/hwmon"
#define _NAME_PATH  "device"
#define _NAME_FILE  "name"
#define _MAX_SENSORS 256
#define _MAX_LABEL   8
/**
 * @brief the name of the SMC name
 **/
static const char _smc_name[] = "applesmc\n";

/**
 * @bief the base directory
 **/
static char _basedir[PATH_MAX] = {};

/**
 * @brief the number of sensors
 **/
static int _sensor_count;

/**
 * @brief the sensors
 **/
static struct {
	char label[_MAX_LABEL];    /*!< the label of the sensor */
	char path[32];             /*!< the relative path of the sensor */
} _sensors[_MAX_SENSORS];

/**
 * @biref find the applesmc 
 * @return status code
 **/
static inline int _find_smc()
{
	DIR* dir = opendir(_HWMON_PATH);
	if(NULL == dir)
	{
		perror("opendir");
		return -1;
	}

	int found = 0;
	
	for(;!found;)
	{
		struct dirent* ent = readdir(dir);
		if(NULL == ent) 
			break;

		if(NULL == ent->d_name || strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
			continue;

		char pathbuf[PATH_MAX];
		snprintf(pathbuf, sizeof(pathbuf), "%s/%s/%s/%s", _HWMON_PATH, ent->d_name, _NAME_PATH, _NAME_FILE);

		FILE* fp = fopen(pathbuf, "r");
		if(NULL == fp) continue;
		char name[sizeof(_smc_name)];
		if(1 != fread(name, sizeof(_smc_name) - 1, 1, fp))
			goto CONTINUE;
		if(fgetc(fp) != -1) 
			goto CONTINUE;

		name[sizeof(name) - 1] = 0;
		if(strcmp(name, _smc_name) != 0) goto CONTINUE;


		found = 1;
		snprintf(_basedir, sizeof(_basedir), "%s/%s/%s", _HWMON_PATH, ent->d_name, _NAME_PATH);
		
CONTINUE:
		fclose(fp);
	}

	if(!found)
	{
		fprintf(stderr, "No AppleSMC found\n");
		return -1;
	}

	printf("Found AppleSMC at %s\n", _basedir);
	
	return 0;
}

/**
 * @brief find the sensors in SMC
 * @return status code
 **/
static inline int _find_sensors()
{
	int i;
	for(i = 0; i < _MAX_SENSORS; i ++)
	{
		/* read the label */
		char path[PATH_MAX];
		snprintf(path, sizeof(path), "%s/temp%d_label", _basedir, i);
		FILE* label_fp = fopen(path, "r");
		if(label_fp == NULL) continue;
		ssize_t ret = fread(_sensors[_sensor_count].label, 1, sizeof(_sensors[_sensor_count].label), label_fp);
		if(ret < 0) continue;
		do
			_sensors[_sensor_count].label[ret] = 0;
		while(ret > 0 && _sensors[_sensor_count].label[-- ret] == '\n');

		snprintf(_sensors[_sensor_count].path, sizeof(_sensors[_sensor_count].path), "temp%d_input", i);

		snprintf(path, sizeof(path), "%s/%s", _basedir, _sensors[_sensor_count].path);
		if(0 == access(path, R_OK)) 
		{
			fprintf(stderr, "Find sensor %s from %s\n", _sensors[_sensor_count].label, path);
			_sensor_count ++;
		}
	}

	return 0;
}

int smc_init()
{
	if(_find_smc() < 0) return -1;
	if(_find_sensors() < 0) return -1;
	return 0;
}
