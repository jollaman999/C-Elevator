#include "floor.h"

// #define DEBUG_INSIDE

// 엘리베이터 내부에 해당되는 기능들
struct INSIDE {
	int floor_button[TOTAL_FLOORS];	// 각 층에 해당되는 버튼들
	int open_button;		// 열림버튼
	int close_button;		// 닫힘버튼
};

struct INSIDE inside;

// 엘리베이터 내부 버튼 on/off 여부
void inside_button_on_off(int floor) {
	if (inside.floor_button[floor - 1] == 1)
		inside.floor_button[floor - 1] = 0;
	else
		inside.floor_button[floor - 1] = 1;
}

// 엘리베이터 내부 버튼 동작 함수 (현재 20층 까지 지원)
int inside_button_press(void) {
	char input[2];	// 사용자 입력
	int input_floor;
	int ok = 1;
#ifdef DEBUG_INSIDE
	int i;
#endif

	input[0] = '\0';

	// 키보드 입력 시간 제한
	while (1) {
		if (_kbhit()) {
			// _getch() 함수 반환형이 int 이므로 char 로 캐스팅 해줌
			// input[1] 은 NULL 문자 채워줌
			input[0] = (char)_getch();
			input[1] = '\0';
		}
		if (wait(1000 / ELEVATOR_SPEED))	// 시간 만료되면 루프 빠져나옴
			break;
	}

	switch (input[0]) {
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			input_floor = atoi(input);
			break;
		case '0':
			input_floor = 10;
			break;
		case '!':
			input_floor = 11;
			break;
		case '@':
			input_floor = 12;
			break;
		case '#':
			input_floor = 13;
			break;
		case '$':
			input_floor = 14;
			break;
		case '%':
			input_floor = 15;
			break;
		case '^':
			input_floor = 16;
			break;
		case '&':
			input_floor = 17;
			break;
		case '*':
			input_floor = 18;
			break;
		case '(':
			input_floor = 19;
			break;
		case ')':
			input_floor = 20;
			break;	// 1~20 층에 해당되는 버튼들
		case 'z':
		case 'Z':
			inside.open_button = 1;
			return ok;
		case 'x':
		case 'X':
			inside.close_button = 1;
			return ok;
		default:
#ifdef DEBUG_INSIDE
			//printf("Error: inside_button_press input failed!\n");
#endif
			ok = 0;
			return ok;
	}

	// 지정된 건물 층 수 보다 큰 값의 버튼 눌렸을 경우에 대한 처리
	if (input_floor > TOTAL_FLOORS) {
#ifdef DEBUG_INSIDE
		printf("Error: inside_button_press floor too big!\n");
#endif
		ok = 0;
		return ok;
	}

	inside_button_on_off(input_floor);

	/* Pressed test code */
#ifdef DEBUG_INSIDE
	printf("\n");

	printf("pressed_button = %s\n", input);

	for (i = 0; i < TOTAL_FLOORS; i++) {
		if (inside.floor_button[i])
			printf("internal_button[%d] = true\n", i + 1);
		else
			printf("internal_button[%d] = false\n", i + 1);
	}

	if (inside.open_button)
		printf("internal_button[open_button] = true\n");
	else
		printf("internal_button[open_button] = false\n");

	if (inside.close_button)
		printf("internal_button[close_button] = true\n");
	else
		printf("internal_button[close_button] = false\n");
#endif

	return ok;
}


// 올라가거나 내려가면서 남은 버튼 있는지 확인하는 함수
int inside_button_check(int cur_floor, int is_up) {
	int i;
	int found = 0;

	if (is_up) {	// 올라가는 방향
		for (i = cur_floor - 1; i < TOTAL_FLOORS; i++) {
			if (inside.floor_button[i])
				found = 1;
		}
	} else {		// 내려가는 방향
		for (i = cur_floor - 1; i >= 0; i--) {
			if (inside.floor_button[i])
				found = 1;
		}
	}

	return found;
}
