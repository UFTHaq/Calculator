// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author : UFTHaq
// Created: 24-12-2023
// Purpose: Simple calculator application using Raylib.

#pragma execution_character_set("utf-8")

#include <iostream>
#include <raylib.h>
#include <vector>
#include <map>
#include <ctime>
#include <cctype>
#include <sstream>


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
//#define FONT_BUTTON_PATH {"Fonts/Ubuntu/Ubuntu-Medium.ttf"}
#define FONT_BUTTON_PATH {"Fonts/Roboto_Mono/static/RobotoMono-Medium.ttf"}
#define FONT_BUTTON_SIZE 250
//#define FONT_CALCULATOR_PATH {"Fonts/Roboto_Mono/static/RobotoMono-Medium.ttf"}
//#define FONT_CALCULATOR_PATH {"Fonts/technology/Technology-Bold.ttf"}
//#define FONT_CALCULATOR_PATH {"Fonts/monogram.ttf"}
//#define FONT_CALCULATOR_PATH {"Fonts/Ubuntu/Ubuntu-Medium.ttf"}
//#define FONT_CALCULATOR_PATH {"Fonts/SansSerifFLF/SansSerifBldFLFCond.otf"}
#define FONT_CALCULATOR_PATH {"Fonts/Oswald/Static/Oswald-regular.ttf"}
#define FONT_CALCULATOR_SIZE 400

#define ICON {"Icons/calculator.png"}
#define TITLE {"CALCULATOR"}
#define DAY_NIGHT {"Icons/DayNight.png"}
#define LABEL {"Icons/Label.png"}

#define CALC_WIDTH 490
#define CALC_HEIGHT 690
#define MAIN_SCR_ON CLITERAL(Color)  { 180, 200, 140, 0xFF }
#define MAIN_SCR_OFF CLITERAL(Color) { 100, 100, 100, 0xFF }
#define MAIN_SCR_BRD CLITERAL(Color) {  20,  20,  20,  100 }

#define CAL_BLK CLITERAL(Color)		 {   5,   5,   5, 255 }
#define CAL_GRA CLITERAL(Color)		 { 120, 110, 100, 255 }
#define CAL_ORA CLITERAL(Color)		 { 170,  50,   5, 255 }
Color CAL_COL_FONT = LIGHTGRAY;
Color ColorHover = SKYBLUE;

//std::string EvaluateExpression(std::string& InputExpression);

struct ButtonComponent {
	float h{};
	float w{};
	float space{};
};

enum MainScreenState {
	OFF = 0,
	ON  = 1
};

enum ButtonState {
	DEFAULT = 0,
	HOVER,
	CLICKED
};

enum ButtonImage {
	BUT_DEL = 0,
	BUT_OFF,
	BUT_MC,
	BUT_MR,
	BUT_MMIN,
	BUT_MPLUS,
	BUT_DIVIDE,
	BUT_SQRT,
	BUT_7,
	BUT_8,
	BUT_9,
	BUT_MULTIPLY,
	BUT_PERCENT,
	BUT_4,
	BUT_5,
	BUT_6,
	BUT_MINUS,
	BUT_PLUSMINUS,
	BUT_1,
	BUT_2,
	BUT_3,
	BUT_PLUS,
	BUT_C_AC_ON,
	BUT_0,
	BUT_COMA,
	BUT_EQUAL
};

class Button {
private:
	float roundness = 0.3f;
	int segments = 10;
	float spacing = 2;
	Vector2 textPos{};
	Vector2 textPosDraw{};
	Color defaultlabelColor{ RAYWHITE };
	Color hoverlabelColor{ BLACK };

public:
	Rectangle rect{};
	Color defaultColor{};
	Color hoverColor{};
	Color clickedColor{};

	ButtonState state{};

	Image image{};
	Texture2D texture{};
	float labelSize{};
	float offsetX{ 8.F };
	float offsetY{};

	Rectangle source{};
	Rectangle dest{};
	size_t icon_index = {};
	Color labelColor{};
	
	// Constructor
	Button(Rectangle r, Color defaultC, Color hoverC, Color clickedC, ButtonImage image, Texture2D texture, float labelSize)
		: rect(r), defaultColor(defaultC), hoverColor(hoverC), clickedColor(clickedC),
		texture(texture), labelSize(labelSize),
		state(DEFAULT) {

		icon_index = static_cast<size_t>(image);
	}

	// Overloading Constructor [offsetY Pos]
	Button(Rectangle r, Color defaultC, Color hoverC, Color clickedC, ButtonImage image, Texture2D texture, float labelSize, float offsetY)
		: rect(r), defaultColor(defaultC), hoverColor(hoverC), clickedColor(clickedC),
		texture(texture), labelSize(labelSize), offsetY(offsetY),
		state(DEFAULT) {

		icon_index = static_cast<size_t>(image);
	}


	void Draw() {
		Color buttonColor{};
		Color labelColor{};

		switch (state)
		{
		case DEFAULT:
			buttonColor = defaultColor;
			labelColor = defaultlabelColor;
			break;
		case HOVER:
			buttonColor = hoverColor;
			labelColor = hoverlabelColor;
			break;
		case CLICKED:
			buttonColor = clickedColor;
			labelColor = hoverlabelColor;
			break;
		default:
			break;
		}

		float icon_size = 380;
		source = { (float)icon_size * icon_index, 0, icon_size, icon_size };

		Rectangle dest = {
			rect.x + offsetX,
			rect.y + offsetY,
			labelSize,
			labelSize
		};

		DrawRectangleRounded(rect, roundness, segments, buttonColor);
		DrawTexturePro(texture, source, dest, Vector2{ 0 }, 0, labelColor);
	}

	bool IsMouseOver() const {
		return CheckCollisionPointRec(GetMousePosition(), rect);
	}

	bool OnClick(std::string& inputExpression, ButtonState& lastOperatorState, Color& MainScreenColor, MainScreenState& MScrState) {
		if (IsMouseOver() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			state = CLICKED;

			std::string buttonLabel = GetButtonLabelFromIndex(icon_index);

			if (buttonLabel == "ON") {
				if (MScrState == ON) {
					inputExpression.clear();
				}
				MScrState = ON;
			}
			else if (buttonLabel == "OFF") {
				MScrState = OFF;
			}
			else if (buttonLabel == "DEL") {
				inputExpression.pop_back();
			}
			else if (buttonLabel == "%") {
				try {
					float value = std::stof(inputExpression);
					value /= 100.F;
					std::string strVal = std::to_string(value);
			
					// Check can Be rounded
					strVal = isCanBeRounded(strVal);
			
					inputExpression = strVal;
				}
				catch (std::invalid_argument) {
					inputExpression = inputExpression;
				}
			}
			else if (buttonLabel == "=") {
				inputExpression = EvaluateExpression(inputExpression);
			}
			else if (buttonLabel == "SQRT") {
				try {
					float value = std::stof(inputExpression);
					value = std::sqrt(value);
					std::string strVal = std::to_string(value);
			
					// Check can Be rounded
					strVal = isCanBeRounded(strVal);
			
					inputExpression = strVal;
				}
				catch (std::invalid_argument) {
					inputExpression = inputExpression;
				}
			}
			else if (inputExpression.size() > 10) {
				inputExpression += "";
			}
			else if (buttonLabel == "+/-") {
				if (inputExpression.front() == '-') {
					inputExpression.erase(inputExpression.begin());
				}
				else {
					inputExpression.insert(inputExpression.begin(), '-');
				}
			}
			else if (buttonLabel == "+") {
				if (inputExpression.back() == '+') {
					inputExpression;
				}
				else {
					inputExpression += buttonLabel;
				}
			}
			else if (buttonLabel == "-") {
				if (inputExpression.back() == '-') {
					inputExpression;
				}
				else {
					inputExpression += buttonLabel;
				}
			}
			else if (buttonLabel == "x") {
				if (inputExpression.back() == 'x') {
					inputExpression;
				}
				else if (inputExpression.back() == '/') {
					inputExpression;
				}
				else {
					inputExpression += buttonLabel;
				}
			}
			else if (buttonLabel == "/") {
				if (inputExpression.back() == '/') {
					inputExpression;
				}
				else if (inputExpression.back() == 'x') {
					inputExpression;
				}
				else {
					inputExpression += buttonLabel;
				}
			}
			else if (buttonLabel == ".") {
				if (inputExpression.find('.') != std::string::npos) {
					inputExpression;
				}
				else {
					inputExpression += buttonLabel;
				}
			}
			else if (buttonLabel == "MC" || buttonLabel == "MR" || buttonLabel == "M-" || buttonLabel == "M+") {
				inputExpression;
			}
			
			else {
				inputExpression += buttonLabel;
			}

			return true;
		}
		else if (IsMouseOver() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			state = CLICKED;
		}
		else if (IsMouseOver()) {
			state = HOVER;
		}
		else {
			state = DEFAULT;
		}

		return false;
	}

	std::string isCanBeRounded(std::string& strVal)
	{
		size_t dotPos = strVal.find('.');
		if (dotPos != std::string::npos) {
			strVal.erase(strVal.find_last_not_of('0') + 1, std::string::npos);

			if (strVal.back() == '.') {
				strVal.pop_back();
			}
		}

		return strVal;
	}

	std::string GetButtonLabelFromIndex(size_t index) const {
		switch (static_cast<ButtonImage>(index))
		{
		case BUT_DEL:			return "DEL";
		case BUT_OFF:			return "OFF";
		case BUT_MC:			return "MC";
		case BUT_MR:			return "MR";
		case BUT_MMIN:			return "M-";
		case BUT_MPLUS:			return "M+";
		case BUT_DIVIDE:		return "/";
		case BUT_SQRT:			return "SQRT";
		case BUT_7:				return "7";
		case BUT_8:				return "8";
		case BUT_9:				return "9";
		case BUT_MULTIPLY:		return "x";
		case BUT_PERCENT:		return "%";
		case BUT_4:				return "4";
		case BUT_5:				return "5";
		case BUT_6:				return "6";
		case BUT_MINUS:			return "-";
		case BUT_PLUSMINUS:		return "+/-";
		case BUT_1:				return "1";
		case BUT_2:				return "2";
		case BUT_3:				return "3";
		case BUT_PLUS:			return "+";
		case BUT_C_AC_ON:		return "ON";
		case BUT_0:				return "0";
		case BUT_COMA:			return ".";
		case BUT_EQUAL:			return "=";
		default:				return "";
		}
	}

	std::string EvaluateExpression(std::string& InputExpression) {
		std::istringstream iss(InputExpression);

		char op;
		double num1, num2;

		iss >> num1;

		while (iss >> op >> num2) {
			switch (op)
			{
			case '+':
				num1 += num2;
				InputExpression = std::to_string(num1);
				InputExpression = isCanBeRounded(InputExpression);
				break;
			case '-':
				num1 -= num2;
				InputExpression = std::to_string(num1);
				InputExpression = isCanBeRounded(InputExpression);
				break;
			case 'x':
				num1 *= num2;
				InputExpression = std::to_string(num1);
				InputExpression = isCanBeRounded(InputExpression);
				break;
			case '/':
				if (num2 != 0) {
					num1 /= num2;
					InputExpression = std::to_string(num1);
					InputExpression = isCanBeRounded(InputExpression);
				}
				else {
					InputExpression = "Error";
				}
				break;

			default:
				break;
			}
		}

		return InputExpression;
	}
};

std::vector<Button> SetupButtons(const Texture2D& LabelTexture);


struct CasioScr {
	float pad{};
	float y{};
	float h{};
};

CasioScr SecondScrSet{ 145, 37, 40 };
CasioScr MainScrSet{ 45, 100, 100 };

Rectangle CasioBaseFrame();
void DrawCalculator(const Font& FontMainStyle, MainScreenState& MScrState);
void DrawMainScreenDisplay(const Font& FontCalculatorStyle, std::string& inputExpression, MainScreenState& MScrState);
void DayNight(int& time_hours, const Texture2D& DayNightTexture);
void DrawSecondScreenDisplay(Font& FontTimeStyle, Texture2D& DayNightTexture);
Rectangle CasioFrontFrame();
Vector2 CasioTitlePos();
Vector2 CasioSeriesPos();

Rectangle SecondScr();
Rectangle SecondScrBorder();

int LiveClock(Font& FontTimeStyle);
void Battery(int& time_hours);

Rectangle MainScr();
Rectangle MainScrBorder();

//std::string EvaluateExpression(std::string& InputExpression);

int main()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
	SetTargetFPS(FPS);

	Image Icon = LoadImage(ICON);
	SetWindowIcon(Icon);
	Font FontMainStyle = LoadFontEx(FONT_PATH, FONT_SIZE, 0, 0);
	Font FontTimeStyle = LoadFontEx(FONT_TIME_PATH, FONT_TIME_SIZE, 0, 0);
	Font FontButtonStyle = LoadFontEx(FONT_BUTTON_PATH, FONT_BUTTON_SIZE, 0, 0);
	Font FontCalculatorStyle = LoadFontEx(FONT_CALCULATOR_PATH, FONT_CALCULATOR_SIZE, 0, 0);
	Image DayNightImage = LoadImage(DAY_NIGHT);
	Texture2D DayNightTexture = LoadTextureFromImage(DayNightImage);
	Image LabelImage = LoadImage(LABEL);
	Texture2D LabelTexture = LoadTextureFromImage(LabelImage);

	Color MainScreenColor = MAIN_SCR_OFF;
	std::string inputExpression = "";
	size_t sizeInput = 0;
	std::string lastInput = "";

	bool startUp = true;

	std::vector<Button> AllButtons = SetupButtons(LabelTexture);

	ButtonState lastOperatorState = DEFAULT;
	MainScreenState MScrState = OFF;

	while (!WindowShouldClose()) 
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw Calculator
		DrawCalculator(FontMainStyle, MScrState);

		// Draw Main Screen Display
		DrawMainScreenDisplay(FontCalculatorStyle, inputExpression, MScrState);

		// Button
		for (auto& button : AllButtons) {
			button.Draw();
			button.OnClick(inputExpression, lastOperatorState, MainScreenColor, MScrState);
		}


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
		UnloadImage(Icon);
		UnloadFont(FontMainStyle);
		UnloadFont(FontTimeStyle);
		UnloadFont(FontButtonStyle);
		UnloadFont(FontCalculatorStyle);
	}

	return 0;
}

std::vector<Button> SetupButtons(const Texture2D& LabelTexture)
{
	float btnH = 52.F;
	float btnW = 68.5F;
	float btnSpc = 18.F;
	ButtonComponent BtnSize{ btnH, btnW, btnSpc };
	std::vector<float> col{};
	for (size_t i = 0; i < 5; i++) {
		col.push_back((float)45 + (i * (BtnSize.w + BtnSize.space)));
	}

	std::vector<float> row{};
	for (size_t i = 0; i < 5; i++) {
		row.push_back((float)320 + (i * (BtnSize.h + BtnSize.space)));
	}

	// ROW1
	Button ButtonDEL	({ col[3], 267 , BtnSize.w,(BtnSize.h * 2 / 3) }, CAL_BLK, SKYBLUE, GREEN, BUT_DEL, LabelTexture, btnH, -9.F);
	Button ButtonOFF	({ col[4], 267, BtnSize.w, (BtnSize.h * 2 / 3) }, CAL_BLK, SKYBLUE, GREEN, BUT_OFF, LabelTexture, btnH, -9.F);

	// ROW2
	Button ButtonMC		({ col[0], row[0], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MC, LabelTexture, btnH);
	Button ButtonMR		({ col[1], row[0], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MR, LabelTexture, btnH);
	Button ButtonMMin	({ col[2], row[0], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MMIN, LabelTexture, btnH);
	Button ButtonMPlus	({ col[3], row[0], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MPLUS, LabelTexture, btnH);
	Button ButtonDevide	({ col[4], row[0], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_DIVIDE, LabelTexture, btnH);

	// ROW3
	Button ButtonSQRT	({ col[0], row[1], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_SQRT, LabelTexture, btnH);
	Button Button7		({ col[1], row[1], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_7, LabelTexture, btnH);
	Button Button8		({ col[2], row[1], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_8, LabelTexture, btnH);
	Button Button9		({ col[3], row[1], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_9, LabelTexture, btnH);
	Button ButtonX		({ col[4], row[1], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MULTIPLY, LabelTexture, btnH);

	// ROW4
	Button ButtonPercent({ col[0], row[2], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_PERCENT, LabelTexture, btnH);
	Button Button4		({ col[1], row[2], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_4, LabelTexture, btnH);
	Button Button5		({ col[2], row[2], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_5, LabelTexture, btnH);
	Button Button6		({ col[3], row[2], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_6, LabelTexture, btnH);
	Button ButtonMin	({ col[4], row[2], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_MINUS, LabelTexture, btnH);

	// ROW5
	Button ButtonPlusMin({ col[0], row[3], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_PLUSMINUS, LabelTexture, btnH);
	Button Button1		({ col[1], row[3], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_1, LabelTexture, btnH);
	Button Button2		({ col[2], row[3], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_2, LabelTexture, btnH);
	Button Button3		({ col[3], row[3], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_3, LabelTexture, btnH);
	Button ButtonPlus	({ col[4], row[3], BtnSize.w, (BtnSize.h * 2 + BtnSize.space) }, CAL_BLK, SKYBLUE, GREEN, BUT_PLUS, LabelTexture, btnH, ((BtnSize.h + BtnSize.space) / 2));

	// ROW6
	Button ButtonON		({ col[0], row[4], BtnSize.w, BtnSize.h }, CAL_ORA, SKYBLUE, GREEN, BUT_C_AC_ON, LabelTexture, btnH);
	Button Button0		({ col[1], row[4], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_0, LabelTexture, btnH);
	Button ButtonComa	({ col[2], row[4], BtnSize.w, BtnSize.h }, CAL_GRA, SKYBLUE, GREEN, BUT_COMA, LabelTexture, btnH);
	Button ButtonEqual	({ col[3], row[4], BtnSize.w, BtnSize.h }, CAL_BLK, SKYBLUE, GREEN, BUT_EQUAL, LabelTexture, btnH);

	std::vector<Button> AllButtons{
		ButtonDEL, ButtonOFF,
		ButtonMC, ButtonMR, ButtonMMin, ButtonMPlus, ButtonDevide,
		ButtonSQRT, Button7, Button8, Button9, ButtonX,
		ButtonPercent, Button4, Button5, Button6, ButtonMin,
		ButtonPlusMin, Button1, Button2, Button3, ButtonPlus,
		ButtonON, Button0, ButtonComa, ButtonEqual,
	};

	return AllButtons;
}

void DrawMainScreenDisplay(const Font& FontCalculatorStyle, std::string& inputExpression, MainScreenState& MScrState)
{

	if (MScrState == ON) {
		const char* displayText = inputExpression.c_str();

		//float fontSizeDivider = 5.75F;
		float fontSizeDivider = 4.5F;
		float fontSize = FONT_CALCULATOR_SIZE / fontSizeDivider;
		float space = 1.5F;
		float rightPad = 10.F;

		const char* Text = displayText;
		Vector2 TextSize = MeasureTextEx(FontCalculatorStyle, Text, fontSize, space);
		Vector2 TextPos = {
			(float)(MainScr().x + MainScr().width - rightPad) - TextSize.x,
			//(float)(MainScr().height / 2) + TextSize.y + 7
			(float)(MainScr().height / 2) + TextSize.y - 7*4
		};

		DrawTextEx(FontCalculatorStyle, Text, TextPos, fontSize, space, BLACK);
		
	}
	else {
		inputExpression.clear();
	}
}



void DrawCalculator(const Font& FontMainStyle, MainScreenState& MScrState)
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

	Color MainScreenColor{};

	switch (MScrState)
	{
	case OFF:
		MainScreenColor = MAIN_SCR_OFF;
		break;
	case ON:
		MainScreenColor = MAIN_SCR_ON;
		break;
	default:
		break;
	}

	DrawRectangleRounded(MainScr(), 0.1f, 10, MainScreenColor);
	//MainScreenColor
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
	if (time_hours >= 6 && time_hours <= 18) {
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



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
