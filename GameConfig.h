#pragma once
class GameConfig {
public:

	 enum  ARROWKEYS{ LEFT = 97, RIGHT = 100, STAY = 115, UP=119,DOWN=120,UPANDRIGHT,UPANDLEFT,DOWNANDRIGHT,DOWNANDLEFT};
	 static int constexpr JUMPSECS = 8;
	 static int constexpr MIN_X = 5;
	 static int constexpr MIN_Y = 1;
	 static int constexpr WIDTH = 90;
	 static int constexpr HEIGHT = 27;
	 static int constexpr ESC = 27;
 };
