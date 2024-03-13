#ifndef CAN_UDS_RESPONSE_REQUEST_SERVICE_H
#define CAN_UDS_RESPONSE_REQUEST_SERVICE_H

#include "main.h"

typedef struct{
	uint16_t spn;
	uint8_t fmi;
	uint8_t status_dtc;
} uds_dtc_format;

//********************************************************************//
typedef enum {		// Подфункции Diagnostic Session Control
	DEFAULT_SESSION									= 0x01,
	PROGRAMMING_SESSION							= 0x02,
	EXTENDED_DIAGNOSTIC_SESSION			= 0x03,
} uds_subfunct_session_control;

typedef enum {		// Подфункции ECU Reset
	HARD_RESET											= 0x01,
	KEY_OFF_ON_RESET								= 0x02,
	SOFT_RESET											= 0x03,
	ENABLE_RAPID_POWER_SHUT_DOWN		= 0x04,
} uds_subfunct_ecu_reset;

typedef enum {		// Подфункции Security Access
	SEED_1													= 0x01,
	SEED_2													= 0x03,
	SEED_3													= 0x05,
	SEED_4													= 0x07,
} uds_subfunct_security_access;

typedef enum {		// Подфункции Communication Control
	ENABLE_RX_AND_TX											= 0x00,
	ENABLE_RX_AND_TX_MORE									= 0x80,
	ENABLE_RX_AND_DISABLE_TX							= 0x01,
	ENABLE_RX_AND_DISABLE_TX_MORE					= 0x81,
	DISABLE_RX_AND_ENABLE_TX							= 0x02,
	DISABLE_RX_AND_ENABLE_TX_MORE					= 0x82,
	DISABLE_RX_AND_TX											= 0x03,
	DISABLE_RX_AND_TX_MORE								= 0x83,
	ENABLE_RX_AND_TX_WITH_EAI							= 0x04,
	ENABLE_RX_AND_TX_WITH_EAI_MORE				= 0x84,
	DISABLE_RX_AND_TX_WITH_EAI						= 0x05,
	DISABLE_RX_AND_TX_WITH_EAI_MORE				= 0x85,
//	VEHICLE_MANUFACTURER_SPECIFIC_FROM		= 0x40,
//	VEHICLE_MANUFACTURER_SPECIFIC_TO			= 0x5F,
//	SYSTEM_SUPPLIER_SPECIFIC_FROM					= 0x60,
//	SYSTEM_SUPPLIER_SPECIFIC_TO						= 0x7E,
} uds_subfunct_communic_control;

typedef enum {		// Подфункции Tester Present
	ZERO_FUNCTION										= 0x00,
	ZERO_FUNCTION_MORE							= 0x80,
} uds_subfunct_tester_present;

typedef enum {		// Подфункции Control DTC Setting
	DTC_SETTING_ON												= 0x01,
	DTC_SETTING_ON_MORE										= 0x81,
	DTC_SETTING_OFF												= 0x02,
	DTC_SETTING_OFF_MORE									= 0x82,
//	VEHICLE_MANUFACTURER_SPECIFIC_FROM		= 0x40,
//	VEHICLE_MANUFACTURER_SPECIFIC_TO			= 0x5F,
//	SYSTEM_SUPPLIER_SPECIFIC_FROM					= 0x60,
//	SYSTEM_SUPPLIER_SPECIFIC_TO						= 0x7E,
} uds_subfunct_control_dtc_setting;

typedef enum {		// Подфункции Read DTC Information
	REP_DTC_BY_STATUS_MASK								= 0x02,
} uds_subfunct_read_dtc_inform;

typedef enum {		// Подфункции Routine Control
	START_ROUTINE													= 0x01,
	STOP_ROUTINE													= 0x02,
	REQUEST_ROUTINE_RESULT								= 0x03,
} uds_subfunct_routine_control;

//********************************************************************//
typedef enum {		// Идентификаторы данныых
	VEHICLE_MANUFACTURE_ECU_SOFTWARE_NUMBER						= 0xF188,		// Номер ПО ЭБУ по каталогу производителя авто (строка)
	VEHICLE_MANUFACTURE_ECU_SOFTWARE_VERSION_NUMBER		= 0xF189,		// Номер версии ПО ЭБУ по каталогу производителя авто (строка)
	SYSTEM_SUPPLIER_IDENTIFIER												= 0xF18A,		// Поставщик системы (строка)
	ECU_MANUFACTURING_DATE														= 0xF18B,		// Дата производства ЭБУ (строка)
	ECU_SERIAL_NUMBER																	= 0xF18C,		// Серийный номер ЭБУ (строка)
	VIN																								= 0xF190,		// ID номер автомобиля (строка)
	VEHICLE_MANUFACTURE_ECU_HARDWARE_NUMBER						= 0xF191,		// Номер аппартного обеспечения (АО) ЭБУ по каталогу авто (строка)
	SYSTEM_SUPPLIER_ECU_HARDWARE_NUMBER								= 0xF192,		// Номер АО ЭБУ по каталогу производителя системы (строка)
	SYSTEM_SUPPLIER_ECU_HARDWARE_VERSION_NUMBER				= 0xF193,		// Номер версии АО ЭБУ по каталогу производителя системы (строка)
	SYSTEM_SUPPLIER_ECU_SOFTWARE_NUMBER								= 0xF194,		// Номер ПО ЭБУ по каталогу производителя системы (строка)
	SYSTEM_SUPPLIER_ECU_SOFTWARE_VERSION_NUMBER				= 0xF195,		// Номер версии ПО ЭБУ каталогу производителя системы (строка)
	EXHAUST_REGULATION_OR_TYPE_APPROVAL_NUMBER				= 0xF196,		// Номер допуска к эксплуатации типа транспортного средства (строка)
	SYSTEM_NAME_OR_ENGINE_TYPE												= 0xF197,		// Имя системы или тип двигателя (строка)
	REPAIR_SHOP_CODE_OR_TESTER_SERIAL_NUMBER					= 0xF198,		// Артикул системы по каталогу замены (строка)
	PROGRAMMING_DATE																	= 0xF199,		// Дата последнего обновления ПО ЭБУ (строка)
	ECU_INSTALLATION_DATE															= 0xF19D,		// Дата установки электронного блока (строка)
	POWER_VOLTAGE																			= 0xF1A2,		// Напряжение бортсети
	STEERING_WHEEL_POSITION														= 0xF1A3,		// Положение рулевого колеса
	TORQUE_SENSOR_VALUE																= 0xF1A4,		// Усилие считанное с датчика усилия
	PHASE_A_POSITION																	= 0xF1A5,		// Положение фазы А
	PHASE_B_POSITION																	= 0xF1A6,		// Положение фазы В
	PHASE_C_POSITION																	= 0xF1A7,		// Положение фазы С
	POWER_COMPONENTS_TEMPERATURE											= 0xF1AB,		// Температура силовых компонентов
	MOTOR_TYPE																				= 0xF1AC,		// Тип электромотора ЭГУР
	STEERING_SHAFT_SENSOR_INSTALLED										= 0xF1AD,		// Признак наличия датчика положения рулевого вала
	MOTOR_POSITION_SENSOR_INSTALLED										= 0xF1AE,		// Признак наличия датчика положения ротора электродвигателя
	ECU_TEMPERATURE_SENSOR_INSTALLED									= 0xF1AF,		// Признак наличия датчика температуры внутри ЭБУ
	TORQUE_SENSOR_INSTALLED														= 0xF1B0,		// Признак наличия датчика момента
	GEARBOX_INSTALLED																	= 0xF1B1,		// Признак наличия редуктора
	ENGINE_RPM																				= 0xF1B2,		// Частота вращения вала двигателя авто
	VEHICLE_SPEED																			= 0xF1B3,		// Скорость автомобиля
	STEERING_WHEEL_CALIBRATED													= 0xF1B4,		// Признак выполнения калибровки рулевого колеса
	STEERING_WHEEL_CALIBRATED_PROGRESS								= 0xF1B5,		// Признак нахождения системы в процессе калибровки рулевого колеса
	STEERING_WHEEL_ZERO_COUNTER												= 0xF1B6,		// Кол-во отчетов по датчику положения рулевого колеса положения руля, припрямолинейном движении
	STEERING_WHEEL_MOST_LEFT_COUNTER									= 0xF1B7,		// Кол-во отчетов по датчику положения рулевого колеса в крайне левом положении руля
	STEERING_WHEEL_MOST_RIGHT_COUNTER									= 0xF1B8,		// Кол-во отчетов по датчику положения рулевого колеса в крайне правом положении руля
	ENGINE_RPM_VALID																	= 0xF1B9,		// Валидность информации о частоте вращения вала двигателя авто, принятой из CAN
	VEHICLE_SPEED_VALID																= 0xF1BA,		// Валидность информации о скорости авто, принятой из CAN
	ENGINE_RPM_CONTROL																= 0xF1BB,		// Частота вращения вала двигателя авто, полученная по внутренней связи от контроллера ЭГУРМ
	VEHICLE_SPEED_CONTROL															= 0xF1BC,		// Скорость авто, полученная по внутренней связи от контроллера ЭГУРМ
	STEERING_WHEEL_ANGLE															= 0xF1BD,		// Угол поворота рулевого колеса
	STEERING_WHEEL_NB_TURNS														= 0xF1BE,		// Количество полных оборотов рулевого колеса
	CAN_SPEED																					= 0xF1EC,		// Скорость передачи по CAN
	CAN_SELF_ADDRESS																	= 0xF1ED,		// Адрес системы на шине CAN (J1939 и UDS)
	CAN_TESTER_ADDRESS																= 0xF1EE,		// Адрес тестирующего устройства на шине CAN (J1939 и UDS)
	FINGER_PRINT																			= 0xF1EF,		// Отпечаток, оставляемый при обновлении ПО (строка)
	LOADER_SOFTWARE_VERSION_NUMBER										= 0xFD03,		// Номер версии ПО загрузчика (строка)
} uds_did;

//********************************************************************//
//********************************************************************//
//********************************************************************//
typedef enum{
	POSITIVE_RESPONSE																	= 0x00,		// Позитивный ответ
	GENERAL_REJECT																		= 0x10,		// Общий отказ
	SERVICE_NOT_SUPPORTED															= 0x11,		// Сервис не поддерживается
	SUB_FUNCTION_NOT_SUPPORTED												= 0x12,		// Подфункция не поддерживается
	INVALID_MESSAGE_LENGTH_FORMAT											= 0x13,		// Неверная длина сообщения или неверный формат
	RESPONSE_TOO_LONG																	= 0x14,		// Ответ слишком длинный
	BUSY_REPEAT_REQUEST																= 0x21,		// Сервер занят - повтор запроса
	CONDITIONS_NOT_CORRECT														= 0x22,		// Условия не верны
	REQUEST_SEQUENCE_ERROR														= 0x24,		// Ошибка последовательности запроса
	NO_RESPONSE_FROM_SUBNET_COMPONENT									= 0x25,		// Нет ответа от компонента подсети
	FAILURE_PREVENTS_EXECUATION_OF_REQUESTED_ACTION		= 0x26,		// Сбой предотвращает выполнения запрошенного действия
	REQUEST_OUT_OF_RANGE															= 0x31,		// Запрос вне диапазона
	SECURITY_ACCESS_DENIED														= 0x33,		// Безопасный доступ запрещен
	INVALID_KEY																				= 0x35,		// Ключ недействителен
	EXCEEDED_NUMBER_OF_ATTEMPTS												= 0x36,		// Превышено количество попыток
	REQUIRED_TIME_DELAY_HAS_NOT_EXPIRED								= 0x37,		// Требуемая задержка не истекла
	UPLOAD_DOWNLOAD_NOT_ACCEPTED											= 0x70,		// Загрузка / выгрузка не принята
	TRANSFER_DATA_SUSPENDED														= 0x71,		// Передача данных приостановлена
	PROGRAMMING_FAILURE																= 0x72,		// Общая ошибка программирования
	WRONG_BLOCK_SEQUANCE_COUNTER											= 0x73,		// Неправильный счетчик последовательности блоков
	REQUEST_RECIEVED_RESPONSE_PENDING									= 0x78,		// Запрос получен правильно - ожидание ответа
	SUB_FUNCTIION_NOT_SUPPORTED_IN_ACTIVE_SESSION			= 0x7E,		// Подфункция не поддерживается в активном сеансе
	SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION						= 0x7F,		// Сервис не поддерживается в активном сеансе
	RPM_TOO_HIGH																			= 0x81,		// RPM слишком высокое
	RPM_TOO_LOW																				= 0x82,		// RPM слишком низкое
	ENGINE_IS_RUNNING																	= 0x83,		// Двигатель запущен
	ENGINE_IS_NOT_RUNNING															= 0x84,		// Двигатель не запущен
	ENGINE_RUN_TIME_TOO_LOW														= 0x85,		// Время работы двигателя слишком низкое
	TEMPERATURE_TOO_HIGH															= 0x86,		// Температура слишком высокая
	TEMPERATURE_TOO_LOW																= 0x87,		// Температура слишком низкая
	SPEED_TO_HIGH																			= 0x88,		// Скорость автомобиля слишком высокая
	SPEED_TOO_LOW																			= 0x89,		// Скорость автомобиля слишком низкая
	THROTTLE_PEDAL_TOO_HIGH														= 0x8A,		// Значение положения дроссельной заслонки / педали акселератора слишком большое
	THROTTLE_PEDAL_TOO_LOW														= 0x8B,		// Значение положения дроссельной заслонки / педали акселератора слишком маленькое
	TRANSMISSION_RANGE_NOT_IN_NEUTRAL									= 0x8C,		// Диапазон трансмиссии не в нейтральном положении
	TRANSMISSION_RANGE_NOT_IN_DEAR										= 0x8D,		// Диапазон трансмиссии не в передаче
	BRAKE_SWICHES_NOT_CLOSED													= 0x8F,		// Не нажата педаль томоза
	SHIFTER_LEVEL_NOT_IN_PARK													= 0x90,		// Рычаг переключения передач не в паркинге
	TORQUE_CONVERTER_CLUTCH_LOCKED										= 0x91,		// Гидротрансформатор сцепления заблокирован
	VOLTAGE_TOO_HIGH																	= 0x92,		// Слишком высокое напряжение
	VOLTAGE_TOO_LOW																		= 0x93,		// Слишком низкое напряжение
} ecu_nrc_codes;

//********************************************************************//
//********************************************************************//
//********************************************************************//
typedef enum{
	STEERING_WHEEL_CALIBRATION			= 0x0100,			// Калибровка положения рулевого колеса
	SET_STEERING_WHEEL_ZERO					= 0x0101,			// Запоминание текущего положения рулевого колеса в качестве нулевой точки
	SET_STEERING_WHEEL_MOST_LEFT		= 0x0102,			// Запоминание текущего значения руля в качестве крайне левого положения
	SET_STEERING_WHEEL_MOST_RIGHT		= 0x0103,			// Запоминание текущего значения руля в качестве крайне правого положения
	ERASE_PROGRAM_FLASH							= 0xFF00,			// Стирание памяти программ электронного блока
	CHECK_PROGRAM_CRC								= 0xFF01,			// Сверка циклической контрольной суммы памяти программ электронного блока
} roitine_identifier;
//********************************************************************//
typedef enum{
	LENGTH_1_ADDRESS_1			= 0x11,
	LENGTH_1_ADDRESS_2			= 0x12,
	LENGTH_1_ADDRESS_3			= 0x13,
	LENGTH_1_ADDRESS_4			= 0x14,
	LENGTH_2_ADDRESS_1			= 0x21,
	LENGTH_2_ADDRESS_2			= 0x22,
	LENGTH_2_ADDRESS_3			= 0x23,
	LENGTH_2_ADDRESS_4			= 0x24,
	LENGTH_3_ADDRESS_1			= 0x31,
	LENGTH_3_ADDRESS_2			= 0x32,
	LENGTH_3_ADDRESS_3			= 0x33,
	LENGTH_3_ADDRESS_4			= 0x34,
	LENGTH_4_ADDRESS_1			= 0x41,
	LENGTH_4_ADDRESS_2			= 0x42,
	LENGTH_4_ADDRESS_3			= 0x43,
	LENGTH_4_ADDRESS_4			= 0x44,
} id_format_address_length;

typedef enum {		// Уровни доступа записи
	UDS_ACCESS_LEVEL_MAINTENANCE_SHOP		= 1,
	UDS_ACCESS_LEVEL_VEHICLE_MANUF			= 2,
	UDS_ACCESS_LEVEL_SYSTEM_SUPPLIER		= 3,
} uds_access_level;

typedef enum {
	ACCESS_DISABLE		= 0,
	ACCESS_ENABLE			= 1,
} access_secur;

typedef enum {
	TESTER_DISABLE		= 0,
	TESTER_ENABLE			= 1,
} state_tester_present;

//********************************************************************//
//********************************************************************//
typedef struct{
	uds_subfunct_session_control	subfunct;
	uint16_t 											P2_server;
	uint16_t 											P_2_server;
	ecu_nrc_codes									nrc_codes;
} struct_session_control;

typedef struct{
	uds_subfunct_ecu_reset				subfunct;
	uint8_t												time_power_off;
	ecu_nrc_codes									nrc_codes;
} struct_ecu_reset;

typedef struct{
	uds_subfunct_security_access	subfunct;
	uint32_t											seed;
	uint32_t											key;
	access_secur									secur_access;
	ecu_nrc_codes									nrc_codes;
} struct_security_access;

typedef struct{
	uds_subfunct_communic_control	subfunct;
	uint8_t												type_communic;
	uint16_t											id_node;
	ecu_nrc_codes									nrc_codes;
} struct_communic_control;

typedef struct{
	uds_subfunct_tester_present		subfunct;
	state_tester_present					state_tester;
	ecu_nrc_codes									nrc_codes;
} struct_tester_present;

typedef struct{
	uds_subfunct_control_dtc_setting	subfunct;
	ecu_nrc_codes											nrc_codes;
} struct_control_dtc_setting;

typedef struct{
	uds_did						did;
	uint8_t						*data;
	uint8_t						len;
	ecu_nrc_codes			nrc_codes;
} struct_read_write_data;

typedef struct{
	uds_dtc_format		*dtc;
	uint8_t						len;
	ecu_nrc_codes			nrc_codes;
} struct_clear_diag_inform;

typedef struct{
	uds_subfunct_read_dtc_inform	subfunct;
	uds_dtc_format								*dtc;
	uint8_t												dtc_status_mask;
	uint8_t												len;
	ecu_nrc_codes									nrc_codes;
} struct_read_dtc_inform;

typedef struct{
	uds_subfunct_routine_control	subfunct;
	roitine_identifier						id_routine;
	uint8_t												*option_control_routine;
	uint8_t												len_memory;
	uint8_t												routine_info;
	uint8_t												*routine_status;
	ecu_nrc_codes									nrc_codes;
} struct_routine_control;

typedef struct{
	struct_routine_control	routine_control[2];
	uint8_t									count_rout;
} struct_full_routine_control;

typedef struct{
	uint8_t										id_data;
	id_format_address_length	id_format_addr_len;
	uint8_t										*data_address_memory;
	uint8_t										*data_size_memory;
	uint8_t										*data_blocks;
	uint8_t										len_addr;
	uint8_t										len_size;
	ecu_nrc_codes							nrc_codes;
} struct_request_dowload;

typedef struct{
	uint8_t						count_blocks;
	uint8_t						*transfer_data;
	uint8_t						len_data;
	ecu_nrc_codes			nrc_codes;
} struct_transfer_data;

typedef struct{
	ecu_nrc_codes			nrc_codes;
} struct_request_transf_exit;

//********************************************************************//
//********************************************************************//
enum {LEN_SID_QUEUE = 10};

typedef struct{
	uint8_t	queue_begin;
	uint8_t queue_end;
	uint8_t queue_sid[LEN_SID_QUEUE];
} uds_request_queue;


extern void (*pUdsNextRequest)(void);

//------------------ Headers from can_uds_request_service.c ----------------//
void RequestDaignosticSessionControl(struct_session_control *session_control);
void RequestEcuReset(struct_ecu_reset *ecu_reset);
void RequestSecurityAccess(struct_security_access *security_access);
void RequestSecurityAccessKey(struct_security_access *security_access);
void RequestCommunicationControl(struct_communic_control *communic_control);
void RequestTesterPresent(struct_tester_present *tester_present);
void RequestControlDtcSetting(struct_control_dtc_setting *control_dtc_setting);
void RequestReadDataByIdentifier(struct_read_write_data *read_data);
void RequestWriteDataByIdentifier(struct_read_write_data *write_data);
void RequestClearDiagnosticInformation(struct_clear_diag_inform *clear_diag_inform);
void RequestReadDtcInformation(struct_read_dtc_inform *read_dtc_inform);
void RequestRoutineControl(struct_routine_control *routine_control);
void RequestRequestDownload(struct_request_dowload *request_dowload);
void RequestTransferData(struct_transfer_data *transfer_data);
void RequestRequestTransferExit();

//------------------ Headers from can_uds_service_send.c ----------------//
void SendDaignosticSessionControl(uds_subfunct_session_control subfunct);
void SendEcuReset(uds_subfunct_ecu_reset subfunct);
void SendSecurityAccess(uds_subfunct_security_access subfunct);
void SendCommunicationControl(uds_subfunct_communic_control	subfunct, uint8_t type_communic, uint16_t id_node);
void SendTesterPresent(uds_subfunct_tester_present subfunct);
void SendControlDtcSetting(uds_subfunct_control_dtc_setting subfunct);
void SendReadDataByIdentifier(uds_did did);
void SendWriteDataByIdentifier(uds_did did, uint8_t* data, const uint8_t len);
void SendClearDiagnosticInformation(uint8_t *data, uint8_t len);
void SendReadDtcInformation(uint8_t dtc_status_mask);
void SendRoutineControl(uds_subfunct_routine_control subfunct, roitine_identifier id_routine, uint8_t *data, uint8_t len);
void SendRequestDownload(uint8_t id_data, id_format_address_length	id_addr_len, uint8_t *data_addr, uint8_t len_addr_data, uint8_t *data_size, uint8_t len_size_data);
void SendTransferData(uint8_t count_blocks, uint8_t *transfer_data, uint8_t len);
void SendRequestTransferExit();
void ProcedyreWriteDataByIdentifier(void);
void SendRequestQueue(void);
void StandInQueueRequest(uint8_t sid);
struct_session_control* GetReqSessionControl();
struct_ecu_reset* GetReqEcuReset();
struct_security_access* GetReqSecurityAccess();
struct_communic_control* GetReqCommunicControl();
struct_tester_present* GetReqTesterPresent();
struct_control_dtc_setting* GetReqControlDtcSetting();
struct_read_write_data* GetReqReadData();
struct_read_write_data* GetReqWriteData();
struct_clear_diag_inform* GetReqClearDiagInform();
struct_read_dtc_inform* GetReqReadDtcInfirm();
struct_routine_control* GetReqRoutineControl();
struct_request_dowload* GetReqRequestDowload();
struct_transfer_data* GetReqTransferData();
void IncReqRoutineControl(void);
void DecrReqRoutineControl(void);

//------------------ Headers from can_uds_response_service.c ----------------//
void ResponseDaignosticSessionControl(uint8_t *data, uint8_t error);
void ResponseEcuReset(uint8_t *data, uint8_t error);
void ResponseSecurityAccess(uint16_t length, uint8_t *data, uint8_t error);
void ResponseCommunicationControl(uint8_t *data, uint8_t error);
void ResponseTesterPresent(uint8_t *data, uint8_t error);
void ResponseControlDtcSetting(uint8_t *data, uint8_t error);
void ResponseReadDataByIdentifier(uint16_t length, uint8_t *data, uint8_t error);
void ResponseWriteDataByIdentifier(uint8_t *data, uint8_t error);
void ResponseClearDiagnosticInformation(uint8_t *data, uint8_t error);
void ResponseReadDtcInformation(uint16_t length, uint8_t *data, uint8_t error);
void ResponseRoutineControl(uint16_t length, uint8_t *data, uint8_t error);
void ResponseRequestDownload(uint16_t length, uint8_t *data, uint8_t error);
void ResponseTransferData(uint16_t length, uint8_t *data, uint8_t error);
void SendSecurAccessKey(struct_security_access *resp_security_access);
void ResponseRequestTransferExit(uint8_t *data, uint8_t error);
void PeriodTesterPresent(void);
uds_subfunct_session_control GetSessionMode(void);
access_secur GetSecurityAccess(void);

//------------------ Headers from can_uds_negative_response_service.c ----------------//
void NegativeResponseDaignosticSessionControl(struct_session_control *resp_session_control);
void NegativeResponseEcuReset(struct_ecu_reset *resp_ecu_reset);
void NegativeResponseSecurityAccess(struct_security_access *resp_security_access);
void NegativeResponseCommunicationControl(struct_communic_control *resp_communic_control);
void NegativeResponseTesterPresent(struct_tester_present *resp_tester_present);
void NegativeResponseControlDtcSetting(struct_control_dtc_setting *resp_control_dtc_setting);
void NegativeResponseReadDataByIdentifier(struct_read_write_data *resp_read_data);
void NegativeResponseWriteDataByIdentifier(struct_read_write_data *resp_write_data);
void NegativeResponseClearDiagnosticInformation(struct_clear_diag_inform *resp_clear_diag_inform);
void NegativeResponseReadDtcInformation(struct_read_dtc_inform *resp_read_dtc_inform);
void NegativeResponseRoutineControl(struct_routine_control *resp_routine_control);
void NegativeResponseRequestDownload(struct_request_dowload *resp_request_dowload);
void NegativeResponseTransferData(struct_transfer_data *resp_transfer_data);
void NegativeResponseRequestTransferExit(struct_request_transf_exit *resp_request_transf_exit);
void StartRequiredTimeDelay(uint8_t start);
uint8_t GetNegativeSession(void);
void AddQueueNegativeSession(uint8_t session);

#endif
