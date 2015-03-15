#include <stdio.h>
#include <stdlib.h>

#include "speed.h"
#include "wait.h"
#include "linux_support.h"

#ifdef LINUX
#include "conio.h" // _getch, _kbhit
#else
#include <conio.h> // _getch, _kbhit
#endif

// #define DEBUG_FLOOR

// 건물 층 높이
#define TOTAL_FLOORS 20

// 각 층에 해당되는 기능들
struct FLOOR {
	int up;		// 올라가는 버튼
	int down;	// 내려가는 버튼
};

struct FLOOR floor[TOTAL_FLOORS];

int floor_button_press(void) {
	char input[2];	// 사용자 입력
	int input_floor;
	int ok = 1;

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
		// UP
		case 'q':
			input_floor = 1;
			floor[0].up = 1;
			break;
		case 'w':
			input_floor = 2;
			floor[1].up = 1;
			break;
		case 'e':
			input_floor = 3;
			floor[2].up = 1;
			break;
		case 'r':
			input_floor = 4;
			floor[3].up = 1;
			break;
		case 't':
			input_floor = 5;
			floor[4].up = 1;
			break;
		case 'y':
			input_floor = 6;
			floor[5].up = 1;
			break;
		case 'u':
			input_floor = 7;
			floor[6].up = 1;
			break;
		case 'i':
			input_floor = 8;
			floor[7].up = 1;
			break;
		case 'o':
			input_floor = 9;
			floor[8].up = 1;
			break;
		case 'p':
			input_floor = 10;
			floor[9].up = 1;
			break;
		case 'Q':
			input_floor = 11;
			floor[10].up = 1;
			break;
		case 'W':
			input_floor = 12;
			floor[11].up = 1;
			break;
		case 'E':
			input_floor = 13;
			floor[12].up = 1;
			break;
		case 'R':
			input_floor = 14;
			floor[13].up = 1;
			break;
		case 'T':
			input_floor = 15;
			floor[14].up = 1;
			break;
		case 'Y':
			input_floor = 16;
			floor[15].up = 1;
			break;
		case 'U':
			input_floor = 17;
			floor[16].up = 1;
			break;
		case 'I':
			input_floor = 18;
			floor[17].up = 1;
			break;
		case 'O':
			input_floor = 19;
			floor[18].up = 1;
			break;
		case 'P':
			input_floor = 20;
			floor[19].up = 1;
			break;

		// DOWN
		case 'a':
			input_floor = 1;
			floor[0].down = 1;
			break;
		case 's':
			input_floor = 2;
			floor[1].down = 1;
			break;
		case 'd':
			input_floor = 3;
			floor[2].down = 1;
			break;
		case 'f':
			input_floor = 4;
			floor[3].down = 1;
			break;
		case 'g':
			input_floor = 5;
			floor[4].down = 1;
			break;
		case 'h':
			input_floor = 6;
			floor[5].down = 1;
			break;
		case 'j':
			input_floor = 7;
			floor[6].down = 1;
			break;
		case 'k':
			input_floor = 8;
			floor[7].down = 1;
			break;
		case 'l':
			input_floor = 9;
			floor[8].down = 1;
			break;
		case ';':
			input_floor = 10;
			floor[9].down = 1;
			break;
		case 'A':
			input_floor = 11;
			floor[10].down = 1;
			break;
		case 'S':
			input_floor = 12;
			floor[11].down = 1;
			break;
		case 'D':
			input_floor = 13;
			floor[12].down = 1;
			break;
		case 'F':
			input_floor = 14;
			floor[13].down = 1;
			break;
		case 'G':
			input_floor = 15;
			floor[14].down = 1;
			break;
		case 'H':
			input_floor = 16;
			floor[15].down = 1;
			break;
		case 'J':
			input_floor = 17;
			floor[16].down = 1;
			break;
		case 'K':
			input_floor = 18;
			floor[17].down = 1;
			break;
		case 'L':
			input_floor = 19;
			floor[18].down = 1;
			break;
		case ':':
			input_floor = 20;
			floor[19].down = 1;
			break;
		default:
#ifdef DEBUG_FLOOR
			printf("Error: floor_button_press input failed!\n");
#endif
			ok = 0;
			return ok;
	}

	// 지정된 건물 층 수 보다 큰 값의 버튼 눌렸을 경우에 대한 처리
	if (input_floor > TOTAL_FLOORS) {
#ifdef DEBUG_FLOOR
		printf("Error: floor_button_press floor too big!\n");
#endif
		ok = 0;
		return ok;
	}

#ifdef DEBUG_FLOOR
	if (floor[input_floor - 1].up)
		printf("floor_button_press Pressed: %d, up\n", input_floor);
	if (floor[input_floor - 1].down)
		printf("floor_button_press Pressed: %d, down\n", input_floor);
#endif

	return ok;
}
