// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma execution_character_set("utf-8")

#include <iostream>
#include <raylib.h>
#include <vector>
#include <map>
#include <ctime>


#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 700
#define UFTHColor CLITERAL(Color) { 0x18, 0x18, 0x18, 0xFF }
#define FPS 60

//#define FONT_PATH {"Fonts/Roboto_Mono/static/RobotoMono-SemiBold.ttf"}
#define FONT_PATH {"Fonts/Orbitron/static/Orbitron-Bold.ttf"}
#define FONT_SIZE 300
//#define FONT_TIME_PATH {"Fonts/monogram.ttf"}
#define FONT_TIME_PATH {"Fonts/technology/Technology-Bold.ttf"}
#define FONT_TIME_SIZE 100
//#define FONT_BUTTON_PATH {"Fonts/Orbitron/static/Orbitron-Bold.ttf"}
#define FONT_BUTTON_PATH {"Fonts/Roboto_Mono/static/RobotoMono-Medium.ttf"}
//#define FONT_BUTTON_PATH {"Fonts/Ubuntu/Ubuntu-Medium.ttf"}
//"D:\Coding\Raylib C++\Calculator\Fonts\Ubuntu\Ubuntu-Medium.ttf"
#define FONT_BUTTON_SIZE 200

#define ICON {"Icons/calculator.png"}
#define TITLE {"CALCULATOR"}
#define DAY_NIGHT {"Icons/DayNight.png"}

#define CALC_WIDTH 490
#define CALC_HEIGHT 690
#define MAIN_SCR_CLR CLITERAL(Color) { 200, 220, 130, 0xFF }
#define MAIN_SCR_BRD CLITERAL(Color) {  20,  20,  20,  100 }

#define CAL_BLK CLITERAL(Color) { 5,   5,  5, 255 }
#define CAL_GRA CLITERAL(Color) { 120, 110, 100, 255 }
#define CAL_ORA CLITERAL(Color) { 170,  50,   5, 255 }
Color CAL_COL_FONT = LIGHTGRAY;
Color ColorHover = SKYBLUE;

struct Button {
	Rectangle rect{};
	Color color{};
	const char* label{};
	float fontsize{};
};

struct CasioScr {
	float pad{};
	float y{};
	float h{};
};

CasioScr SecondScrSet{ 145, 37, 40 };
CasioScr MainScrSet{ 45, 100, 100 };

Rectangle CasioBaseFrame();
void DayNight(int& time_hours, const Texture2D& DayNightTexture);
void DrawSecondScreenDisplay(Font& FontTimeStyle, Texture2D& DayNightTexture);
void DrawCalculator(const Font& FontMainStyle);
Rectangle CasioFrontFrame();
Vector2 CasioTitlePos();
Vector2 CasioSeriesPos();

Rectangle SecondScr();
Rectangle SecondScrBorder();

int LiveClock(Font& FontTimeStyle);
void Battery(int& time_hours);

Rectangle MainScr();
Rectangle MainScrBorder();

void DrawAllButtons(Font& FontButtonStyle);

void isHoverOver(const Rectangle& ButtonPos, Color& ButtonColor, Color& TextColor);

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
	SetTargetFPS(FPS);

	SetWindowIcon(LoadImage(ICON));
	Font FontMainStyle = LoadFontEx(FONT_PATH, FONT_SIZE, 0, 0);
	Font FontTimeStyle = LoadFontEx(FONT_TIME_PATH, FONT_TIME_SIZE, 0, 0);
	Font FontButtonStyle = LoadFontEx(FONT_BUTTON_PATH, FONT_BUTTON_SIZE, 0, 0);
	Image DayNightImage = LoadImage(DAY_NIGHT);
	Texture2D DayNightTexture = LoadTextureFromImage(DayNightImage);

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw Calculator
		DrawCalculator(FontMainStyle);

		// Button
		DrawAllButtons(FontButtonStyle);

		// FPS
		DrawFPS(40, 25);

		// Draw Second Screen Display
		DrawSecondScreenDisplay(FontTimeStyle, DayNightTexture);

		EndDrawing();
	}

	// Unload AllAssests
	{
		UnloadTexture(DayNightTexture);
		UnloadImage(DayNightImage);
		UnloadFont(FontMainStyle);
		UnloadFont(FontTimeStyle);
		UnloadFont(FontButtonStyle);
	}

	return 0;
}

void DrawCalculator(const Font& FontMainStyle)
{
	// Base
	DrawRectangleRounded(CasioBaseFrame(), 0.1f, 2, DARKGRAY);
	// Front
	DrawRectangleRounded(CasioFrontFrame(), 0.1f, 2, UFTHColor);
	// Merk
	DrawTextEx(FontMainStyle, "CASIO", CasioTitlePos(), (FONT_SIZE / 11), 2, LIGHTGRAY);
	// Second Screen Border
	DrawRectangleRounded(SecondScrBorder(), 0.15f, 10, MAIN_SCR_BRD);
	// Second Screen
	DrawRectangleRounded(SecondScr(), 0.15f, 10, DARKBROWN);
	// Series
	DrawTextEx(FontMainStyle, "MX-8B", CasioSeriesPos(), (FONT_SIZE / 20.f), 2, LIGHTGRAY);
	// Main Screen Border
	DrawRectangleRounded(MainScrBorder(), 0.10f, 10, { 50,50,50,200 });
	// Main Screen
	DrawRectangleRounded(MainScr(), 0.1f, 10, MAIN_SCR_CLR);
}

void DrawSecondScreenDisplay(Font& FontTimeStyle, Texture2D& DayNightTexture)
{
	// Time
	int time_hours = LiveClock(FontTimeStyle);

	// Battery
	Battery(time_hours);

	// DayNight
	DayNight(time_hours, DayNightTexture);
}

void DayNight(int& time_hours, const Texture2D& DayNightTexture)
{
	size_t icon_index = 0;
	if (time_hours > 6 && time_hours < 18) {
		icon_index = 0;
	}
	else {
		icon_index = 1;
	}

	float pad = 7.5f;
	float icon_size = 400;
	Rectangle dest = {
		(float)CALC_WIDTH - SecondScrSet.pad - (pad * 3.5f),
		(float)CasioBaseFrame().y + SecondScrSet.y + pad,
		(float)SecondScrSet.h - (pad * 2),
		(float)SecondScrSet.h - (pad * 2)
	};

	Rectangle source = { icon_size * icon_index, 0, icon_size, icon_size };

	DrawTexturePro(DayNightTexture, source, dest, Vector2{ 0 }, 0, GRAY);
}


Rectangle CasioBaseFrame()
{
	float pad = 5;
	Rectangle CasioBase{
		pad, pad, WINDOW_WIDTH - (pad * 2), WINDOW_HEIGHT - (pad * 2)
	};

	return CasioBase;
}

Rectangle CasioFrontFrame()
{
	float pad = 12.5f;
	Rectangle CasioFrontFrame{
		pad, pad, WINDOW_WIDTH - (pad * 2), WINDOW_HEIGHT - (pad * 2)
	};
	return CasioFrontFrame;
}

Vector2 CasioTitlePos()
{
	Vector2 CasioTitlePos{
		CasioBaseFrame().x + 35, CasioBaseFrame().y + 42.5f
	};

	return CasioTitlePos;
}

Vector2 CasioSeriesPos()
{
	Vector2 CasioSeriesPos{
		CasioBaseFrame().x + 35 + 350, CasioBaseFrame().y + 48.5f
	};
	return CasioSeriesPos;
}

Rectangle SecondScr()
{
	Rectangle SecondScr{
		(float)CasioBaseFrame().x + SecondScrSet.pad,
		(float)CasioBaseFrame().y + SecondScrSet.y,
		(float)CALC_WIDTH - (SecondScrSet.pad * 2),
		(float)SecondScrSet.h
	};
	return SecondScr;
}

Rectangle SecondScrBorder()
{
	float s_pad = 3;  // sidepad
	float h_pad = 2;  // top-bottom pad

	Rectangle SecondScrBorder{
		(float)CasioBaseFrame().x + SecondScrSet.pad - s_pad,
		(float)CasioBaseFrame().y + SecondScrSet.y - h_pad,
		(float)CALC_WIDTH - ((SecondScrSet.pad - s_pad) * 2),
		(float)SecondScrSet.h + (h_pad * 2)
	};
	return SecondScrBorder;
}

void Battery(int& time_hours)
{
	// Draw The Battery Frame
	float left_pad = 10;
	float h_pad = 13.f;
	float batteryLength = 30;
	Rectangle BatteryFrame{
		(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad,
		(float)CasioBaseFrame().y + SecondScrSet.y + h_pad,
		(float)batteryLength,
		(float)SecondScrSet.h - (h_pad * 2)
	};
	DrawRectangleRoundedLines(BatteryFrame, 0.1f, 10, 2, GRAY);

	// Draw The Battery Anode
	float y_pos_anode = 16;
	Rectangle Anode{
		(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad + batteryLength,
		(float)CasioBaseFrame().y + SecondScrSet.y + y_pos_anode,
		(float)5,
		(float)SecondScrSet.h - (y_pos_anode * 2)
	};
	DrawRectangleRounded(Anode, 0.5f, 10, GRAY);

	// Draw the cell
	Color cell_color = GRAY;
	int battery_cell = 0;
	if (time_hours >= 1 && time_hours < 10) {
		battery_cell = 4;
	}
	else if (time_hours >= 10 && time_hours < 13) {
		battery_cell = 3;
	}
	else if (time_hours >= 13 && time_hours < 17) {
		battery_cell = 2;
	}
	else if (time_hours >= 17 && time_hours < 20) {
		battery_cell = 1;
	}
	else {
		cell_color = RED;
		battery_cell = 1;
		// buat kedip kedip
	}

	float cell_width = batteryLength / 5.4f;
	float y_cell_pad = 14.5f;
	float side_pad = 1.5f;
	for (int i = 0; i < battery_cell; i++) {
		Rectangle Cell{
			(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad + (i * cell_width) + (i * side_pad) + side_pad,
			(float)CasioBaseFrame().y + SecondScrSet.y + y_cell_pad,
			(float)cell_width,
			(float)SecondScrSet.h - (y_cell_pad * 2)
		};
		DrawRectangleRounded(Cell, 0.5f, 10, cell_color);
	}
}

int LiveClock(Font& FontTimeStyle)
{
	time_t currentTime = time(nullptr);

	if (currentTime == -1) {
		std::cerr << "INFO: [FAILED] Getting current time." << std::endl;
		return -1;
	}

	struct tm timeInfo;

	if (localtime_s(&timeInfo, &currentTime) != 0) {
		std::cerr << "INFO: [FAILED] Converting time to local time." << std::endl;
		return -1;
	}

	int time_hours = timeInfo.tm_hour;

	float font_size = FONT_TIME_SIZE / 4.5f;
	char timeString[9];
	strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeInfo);

	Vector2 TimePos = MeasureTextEx(FontTimeStyle, TextFormat("%s", timeString), font_size, 0);

	Vector2 TimePosDraw{ (float)CALC_WIDTH / 2 - (TimePos.x / 2) + 2.5f, (float)(SecondScrSet.h / 2) + (float)(TimePos.y / 0.675f) };
	DrawTextEx(
		FontTimeStyle,
		TextFormat("%s", timeString),
		TimePosDraw,
		font_size,
		2.5f,
		GRAY
	);

	return time_hours;
}

Rectangle MainScr() 
{

	Rectangle MainScr{
		(float)CasioBaseFrame().x + MainScrSet.pad,
		(float)CasioBaseFrame().y + MainScrSet.y,
		(float)CALC_WIDTH - (MainScrSet.pad * 2),
		(float)MainScrSet.h
	};

	return MainScr;
}

Rectangle MainScrBorder() 
{
	int s_pad = 8; // sidepad
	int h_pad = 6;  // top-bottom pad

	Rectangle MainScrBorder{
		(float)CasioBaseFrame().x + MainScrSet.pad - s_pad,
		(float)CasioBaseFrame().y + MainScrSet.y - h_pad,
		(float)CALC_WIDTH - ((MainScrSet.pad - s_pad) * 2),
		(float)MainScrSet.h + (h_pad * 2)
	};

	return MainScrBorder;
}

void DrawAllButtons(Font& FontButtonStyle)
{
	// Assets
	std::vector<const char*> TopButtons{ "MU", "OFF" };

	std::vector<std::vector<const char*>> AllButtons
	{
		{ "MC", "sqrt", "%%", "+/-", "AC/ON"},
		{ "MR", "7", "4", "1", "0" },
		{ "M-", "8", "5", "2", "." },
		{ "M+", "9", "6", "3", "=" },
		{ "/", "x", "-", "+" }
	};

	// All Buttons
	float spaceW = 17;
	float spaceH = 17;
	float buttonWidth = (490 / 5) - 30.f;
	float buttonHeight = 52.5;

	// TopButtons
	float topButtonH = buttonHeight / 1.65f;

	// Button Start Position
	float xStart = 45;
	float yStart = 320;

	// Draw Button
	Rectangle ButtonPos{};
	float roundness = 0.35f;
	int segments = 10;
	Color ButtonColor{};

	// Draw Label Text
	Vector2 TextPosDraw{};
	float fontSizeDevider = 5.f;
	Vector2 TextPos{};
	float TextSpace = 0;
	Color TextColor = CAL_COL_FONT;

	const char* symbol = nullptr;

	for (size_t i = 0; i < TopButtons.size(); i++) {
		symbol = TopButtons[i];
		ButtonColor = CAL_BLK;

		fontSizeDevider = 6.f;

		ButtonPos = Rectangle
		{
			xStart + ((3 + i) * buttonWidth) + ((3 + i) * spaceW),
			yStart - topButtonH - spaceH,
			buttonWidth,
			topButtonH
		};

		TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

		TextPosDraw = Vector2
		{
			ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
			ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2 
		};

		isHoverOver(ButtonPos, ButtonColor, TextColor);

		DrawRectangleRounded(ButtonPos, roundness, segments, ButtonColor);

		DrawTextEx(
			FontButtonStyle,
			TextFormat(symbol),
			TextPosDraw,
			(float)FONT_BUTTON_SIZE / fontSizeDevider,
			TextSpace,
			TextColor
		);

	}

	for (size_t i = 0; i < AllButtons.size(); i++) {
		for (size_t j = 0; j < AllButtons[i].size(); j++) {
			symbol = AllButtons[i][j];

			if (symbol == "AC/ON") {
	 			ButtonColor = CAL_ORA;
			
				fontSizeDevider = 8.f;

				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					buttonHeight
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				TextPosDraw = Vector2
				{
					ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
					ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
				};

			}
			else if (
				symbol == "." ||
	 			symbol == "0" ||
	 			symbol == "1" ||
	 			symbol == "2" ||
	 			symbol == "3" ||
	 			symbol == "4" ||
	 			symbol == "5" ||
	 			symbol == "6" ||
	 			symbol == "7" ||
	 			symbol == "8" ||
	 			symbol == "9") {
	 		
	 			ButtonColor = CAL_GRA;
			
				fontSizeDevider = 3.75f;
				
				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					buttonHeight
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				if (symbol != ".") {
					fontSizeDevider = 3.75f;

					TextPosDraw = Vector2
					{
						ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
						ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
					};
				}
				else {
					fontSizeDevider = 3.5f;

					TextPosDraw = Vector2
					{
						ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
						ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 13.5f
					};
				}

			}
			else if (symbol == "+") {
				ButtonColor = CAL_BLK;
			
				fontSizeDevider = 3.5f;
				
				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					(buttonHeight * 2) + spaceH
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				TextPosDraw = Vector2
				{
					ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
					ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
				};

			}
			else if (symbol == "sqrt") {
				ButtonColor = CAL_BLK;

				fontSizeDevider = 7.5f;
				
				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					buttonHeight
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				TextPosDraw = Vector2
				{
					ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
					ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
				};

			}
			else if (
				symbol == "%%" ||
				symbol == "-" ||
				symbol == "x" ||
				symbol == "=") {

				ButtonColor = CAL_BLK;

				fontSizeDevider = 3.5f;
				
				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					buttonHeight
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				if (symbol != "x") {
					fontSizeDevider = 3.5f;

					TextPosDraw = Vector2
					{
						ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
						ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
					};
				}
				else {
					fontSizeDevider = 4.f;

					TextPosDraw = Vector2
					{
						ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2) + 2.5f,
						ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
					};
				}

			}
			else {
	 			ButtonColor = CAL_BLK;
			
				fontSizeDevider = 5.25f;

				ButtonPos = Rectangle
				{
					xStart + (i * buttonWidth) + (i * spaceW),
					yStart + (j * buttonHeight) + (j * spaceH),
					buttonWidth,
					buttonHeight
				};

				TextPos = MeasureTextEx(FontButtonStyle, TextFormat(symbol), (float)(FONT_BUTTON_SIZE / fontSizeDevider), 0);

				TextPosDraw = Vector2
				{
					ButtonPos.x + (ButtonPos.width / 2) - (float)(TextPos.x / 2),
					ButtonPos.y + (ButtonPos.height / 2) - (float)(TextPos.y / 2) - 2
				};
				
			}

			isHoverOver(ButtonPos, ButtonColor, TextColor);

			DrawRectangleRounded(ButtonPos, roundness, segments, ButtonColor);

			DrawTextEx(
				FontButtonStyle,
				TextFormat(symbol),
				TextPosDraw,
				(float)FONT_BUTTON_SIZE / fontSizeDevider,
				TextSpace,
				TextColor
			);
		}
	}
}

void isHoverOver(const Rectangle& ButtonPos, Color& ButtonColor, Color& TextColor)
{
	if (CheckCollisionPointRec(GetMousePosition(), ButtonPos)) {
		ButtonColor = ColorHover;
		TextColor = BLACK;
	}
	else {
		TextColor = CAL_COL_FONT;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
