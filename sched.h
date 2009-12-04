typedef enum {
	IDLE,
	PULSING,
	IGNORING,
	WAITING,
	RECEIVING
} state_t;

extern state_t state;
