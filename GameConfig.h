#pragma once
class GameConfig {
public:

	 enum  ARROWKEYS{ LEFT = 97, RIGHT = 100, STAY = 115, UP=119,DOWN=120,UPANDRIGHT,UPANDLEFT,DOWNANDRIGHT,DOWNANDLEFT};
	 static int constexpr JUMPSECS = 4;
	 static int constexpr MIN_X = 5;
	 static int constexpr MIN_Y = 1;
	 static int constexpr WIDTH = 100;
	 static int constexpr HEIGHT = 32;
	 static int constexpr ESC = 27;
	 static int constexpr FLOORDIFF = 4;
	 enum FLOORS{
		 FLOOR1=GameConfig::MIN_Y+GameConfig::HEIGHT-2,
		 FLOOR2= GameConfig::MIN_Y + GameConfig::HEIGHT - (2+FLOORDIFF),
		 FLOOR3 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*2),
		 FLOOR4 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*3),
		 FLOOR5 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*4),
		 FLOOR6 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*5),
		 FLOOR7 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*6),
		 FLOOR8 = GameConfig::MIN_Y + GameConfig::HEIGHT - (2 + FLOORDIFF*7)
	 };
 };
