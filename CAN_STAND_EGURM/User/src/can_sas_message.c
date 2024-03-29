#include "can_sas_message.h"

SasCsMessageStruc SasCsMessage;

void CanSasCsMessage(uint8_t* recieve_data)
{
	SasCsMessage.steering_wheel_angle = recieve_data[0] + (recieve_data[1] << 8);
	
	SasCsMessage.steering_wheel_angle_range_counter = (recieve_data[2] & 0x3F);
	SasCsMessage.steering_wheel_angle_range_counter_type = ((recieve_data[2] & 0xC0) >> 6);
	
	SasCsMessage.steering_wheel_angle_range = recieve_data[4] + (recieve_data[5] << 8);
	
	SasCsMessage.steering_angle_sensor_active_mode = (recieve_data[6] & 0x03);
	SasCsMessage.steering_angle_sensor_calibrated = ((recieve_data[6] & 0x0C) >> 2);
	
	SasCsMessage.message_counter = (recieve_data[7] & 0x0F);
	SasCsMessage.message_checksum = ((recieve_data[7] & 0xF0) >> 4);
	
	SendAngleEgurm(SasCsMessage.steering_wheel_angle);
}
