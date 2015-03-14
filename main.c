#include "elevator.h"

int main(void) {
	printf("\n				Elevator v1.0		\n");
	printf("						- By jollaman999 -\n");

	elevator_print_manual();
	printf("아무키나 누르면 프로그램을 시작합니다\n");
	_getch();

	elevator_power_on();

	while (1) {
		inside_button_press();	// 버튼 입력 대기
		elevator_up();
		elevator_down();
	}

	return 0;
}
