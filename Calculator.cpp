﻿// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author : UFTHaq
// Created: 24-12-2023
// Purpose: Simple calculator application using Raylib.

#pragma execution_character_set("utf-8")

#include <iostream>
#include <string>
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
#define LABEL {"Icons/LABEL3.png"}

#define CALC_WIDTH 490
#define CALC_HEIGHT 690

#define SECOND_SCR_CLR CLITERAL(Color) { 70, 60, 45, 255 }

#define MAIN_SCR_ON CLITERAL(Color)  { 180, 200, 140, 0xFF }
#define MAIN_SCR_OFF CLITERAL(Color) { 100, 100, 100, 0xFF }
#define MAIN_SCR_BRD CLITERAL(Color) {  20,  20,  20,  100 }
#define MAIN_SCR_BRD_CLR CLITERAL(Color) { 45, 45, 45, 200 }

#define CAL_BLK CLITERAL(Color)		 {  20,  20,  20, 255 }
#define CAL_GRA CLITERAL(Color)		 { 110, 100,  90, 255 }
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

		float listLightSIDE = 0.6F;
		float listLightUPDOWN = 0.5F;
		float scale = 1.75F;
		Rectangle listLight = {
			rect.x - listLightSIDE, rect.y + (listLightUPDOWN / scale), rect.width, rect.height - (listLightUPDOWN * scale)
		};

		float listShadowUP = 2.F;
		float listShadowDOWN = 6.F;
		Rectangle listShadow = {
			rect.x - listShadowUP, rect.y - listShadowUP, rect.width + listShadowDOWN, rect.height + (listShadowDOWN / 1.25F)
		};

		DrawRectangleRounded(listShadow, roundness, segments, BLACK);
		DrawRectangleRounded(listLight, roundness, segments, WHITE);
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
			size_t BUTTON = static_cast<ButtonImage>(icon_index);
			size_t sizeInput = inputExpression.size();

			if (MScrState == OFF && BUTTON == BUT_C_AC_ON) {
				MScrState = ON;
			}
			
			else {
				switch (BUTTON)
				{
				case BUT_C_AC_ON:
					if (MScrState == ON) {
						inputExpression.clear();
					}
					MScrState = ON;
					break;

				case BUT_OFF:
					MScrState = OFF;
					break;

				case BUT_DEL:
					if (sizeInput == 0) {
						inputExpression;
					}
					else {
						inputExpression.pop_back();
					}
					break;

				case BUT_MULTIPLY:
					if (sizeInput == 0) {
						inputExpression;
					}
					else if (inputExpression.back() == 'x') {
						inputExpression;
					}
					else if (inputExpression.back() == '/') {
						inputExpression;
					}
					else {
						inputExpression += buttonLabel;
					}
					break;

				case BUT_DIVIDE:
					if (sizeInput == 0) {
						inputExpression;
					}
					else if (inputExpression.back() == '/') {
						inputExpression;
					}
					else if (inputExpression.back() == 'x') {
						inputExpression;
					}
					else {
						inputExpression += buttonLabel;
					}
					break;

				case BUT_PERCENT:
					try {
						float value = std::stof(inputExpression);
						value /= 100;
						std::string strVal = std::to_string(value);

						// Check can Be rounded
						strVal = isCanBeRounded(strVal);

						inputExpression = strVal;
					}
					catch (std::invalid_argument) {
						inputExpression = inputExpression;
					}
					break;

				case BUT_SQRT:
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
					break;

				case BUT_PLUSMINUS:
					if (sizeInput == 0) {
						inputExpression;
					}
					else if (inputExpression.front() == '-') {
						inputExpression.erase(inputExpression.begin());
					}
					else {
						inputExpression.insert(inputExpression.begin(), '-');
					}
					break;

				case BUT_EQUAL:
					inputExpression = EvaluateExpression(inputExpression);
					break;

				case BUT_MC:
				case BUT_MR:
				case BUT_MMIN:
				case BUT_MPLUS:
					break;

				case BUT_COMA:
					if (sizeInput == 0) {
						inputExpression;
					}
					else if (
						inputExpression.find('/') != std::string::npos ||
						inputExpression.find('x') != std::string::npos ||
						inputExpression.find('-') != std::string::npos ||
						inputExpression.find('+') != std::string::npos
						) {

						size_t count = 0;
						for (size_t i = 0; i < sizeInput; i++) {
							if (inputExpression.at(i) == '.') {
								count++;
							}
						}

						if (count < 2) {
							inputExpression += buttonLabel;
						}

					}
					else if (inputExpression.find(".") != std::string::npos) {
						inputExpression;
					}
					else if (inputExpression.back() == '.') {
						inputExpression;
					}
					else {
						inputExpression += buttonLabel;
					}
					break;

				case BUT_PLUS:
					if (sizeInput == 0) {
						inputExpression;
					}
					else {
						inputExpression += buttonLabel;
					}
					break;

				case BUT_MINUS:
					if (sizeInput == 0) {
						inputExpression;
					}
					else {
						inputExpression += buttonLabel;
					}
					break;

				case BUT_0:
				case BUT_1:
				case BUT_2:
				case BUT_3:
				case BUT_4:
				case BUT_5:
				case BUT_6:
				case BUT_7:
				case BUT_8:
				case BUT_9:
					if (sizeInput > 10) {
						inputExpression += "";
					}
					else {
						inputExpression += buttonLabel;
					}
					break;
			
				default:
					break;
				}

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

CasioScr SecondScrSet{ 145, 37, 42.5F };
CasioScr MainScrSet{ 45, 100, 100 };

Rectangle CasioBaseFrame();
void DrawCalculator(const Font& FontMainStyle, MainScreenState& MScrState);
void DrawMainScreenDisplay(const Font& FontCalculatorStyle, std::string& inputExpression, MainScreenState& MScrState);
void DayNight(int& time_hours, const Texture2D& DayNightTexture);
void DrawSecondScreenDisplay(Font& FontTimeStyle, Texture2D& DayNightTexture);
Rectangle CasioFrontFrame();
Rectangle CasioFrontFrameLight();
Rectangle CasioFrontFrameShadow();
Vector2 CasioTitlePos();
Vector2 CasioSeriesPos();

Rectangle SecondScrBorder();
Rectangle SecondScrBorderLight();
Rectangle SecondScr();

int LiveClock(Font& FontTimeStyle);
void Battery(int& time_hours);

Rectangle MainScrBorder();
Rectangle MainScrShadow();
Rectangle MainScrLight();
Rectangle MainScr();

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
		//DrawFPS(40, 25);

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
			(float)(MainScr().height / 2) + TextSize.y - (7 * 4.25F)
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
	DrawRectangleRounded(CasioBaseFrame(), 0.1F, 2, DARKGRAY);
	// Front Light
	DrawRectangleRounded(CasioFrontFrameLight(), 0.1F, 2, WHITE);
	// Front Shadow
	DrawRectangleRounded(CasioFrontFrameShadow(), 0.1F, 2, BLACK);
	// Front
	DrawRectangleRounded(CasioFrontFrame(), 0.1F, 2, UFTHColor);
	// Merk
	DrawTextEx(FontMainStyle, "CASIO", CasioTitlePos(), (FONT_SIZE / 11), 2, LIGHTGRAY);
	// Second Screen Border Light
	DrawRectangleRounded(SecondScrBorderLight(), 0.15F, 10, WHITE);
	// Second Screen Border
	DrawRectangleRounded(SecondScrBorder(), 0.15F, 10, BLACK);
	// Second Screen
	DrawRectangleRounded(SecondScr(), 0.15F, 10, SECOND_SCR_CLR);
	// Series
	DrawTextEx(FontMainStyle, "MX-8B", CasioSeriesPos(), (FONT_SIZE / 20.F), 2, LIGHTGRAY);
	// Main Screen Border
	DrawRectangleRounded(MainScrBorder(), 0.10F, 10, MAIN_SCR_BRD_CLR);
	// Main Screen Light
	DrawRectangleRounded(MainScrLight(), 0.10F, 10, WHITE);
	// Main Screen Shadow
	DrawRectangleRounded(MainScrShadow(), 0.10F, 10, BLACK);

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

	// Main Screen
	DrawRectangleRounded(MainScr(), 0.1f, 10, MainScreenColor);
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
	Color DayNightColor = GRAY;

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
		(float)CALC_WIDTH - SecondScrSet.pad - (pad * 4.F),
		(float)CasioBaseFrame().y + SecondScrSet.y + pad,
		(float)SecondScrSet.h - (pad * 2),
		(float)SecondScrSet.h - (pad * 2)
	};

	Rectangle source = { icon_size * icon_index, 0, icon_size, icon_size };

	DrawTexturePro(DayNightTexture, source, dest, Vector2{ 0 }, 0, DayNightColor);
}


Rectangle CasioBaseFrame()
{
	float pad = 5;
	Rectangle CasioBase{
		pad, 
		pad, 
		WINDOW_WIDTH - (pad * 2),
		WINDOW_HEIGHT - (pad * 2)
	};

	return CasioBase;
}

Rectangle CasioFrontFrame()
{
	float pad = 12.5F;
	Rectangle CasioFrontFrame{
		pad, 
		pad, 
		WINDOW_WIDTH - (pad * 2), 
		WINDOW_HEIGHT - (pad * 2)
	};
	return CasioFrontFrame;
}

Rectangle CasioFrontFrameLight()
{
	float pad = 12.5F;
	float sideLightPad = 1.F;
	float topLightPad = 2.F;
	Rectangle CasioFrontFrame{
		pad - sideLightPad, 
		pad + (topLightPad/5.F),
		WINDOW_WIDTH - (pad * 2), 
		WINDOW_HEIGHT - (pad * 2) - topLightPad
	};
	return CasioFrontFrame;
}

Rectangle CasioFrontFrameShadow() 
{
	float pad = 12.5F;
	float sideShadowPad = 3.5F;
	float topShadowPad = 0.35F;
	Rectangle CasioFrontFrame{
		pad + sideShadowPad,
		pad + (topShadowPad / 5.F),
		WINDOW_WIDTH - (pad * 2),
		WINDOW_HEIGHT - (pad * 2) + topShadowPad
	};
	return CasioFrontFrame;
}

Vector2 CasioTitlePos()
{
	float UPDOWN = 44.F;
	Vector2 CasioTitlePos{
		CasioBaseFrame().x + 35, CasioBaseFrame().y + UPDOWN
	};

	return CasioTitlePos;
}

Vector2 CasioSeriesPos()
{
	float UPDOWN = 50.F;
	Vector2 CasioSeriesPos{
		CasioBaseFrame().x + 35 + 350, CasioBaseFrame().y + UPDOWN
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
	float s_pad = 1.65F;  // sidepad
	float h_pad = 1.55F;  // top-bottom pad
	float lightPadSide = 0.1F;
	Rectangle SecondScrBorder{
		(float)CasioBaseFrame().x + SecondScrSet.pad - s_pad,
		(float)CasioBaseFrame().y + SecondScrSet.y - h_pad,
		(float)CALC_WIDTH - ((SecondScrSet.pad - s_pad - lightPadSide) * 2),
		(float)SecondScrSet.h + (h_pad * 2)
	};
	return SecondScrBorder;
}

Rectangle SecondScrBorderLight()
{
	float s_pad = 1.45F;  // sidepad
	float h_pad = 1.25F;  // top-bottom pad
	float lightPadSide = 0.45F;
	float lightPadUPDOWN = 0.65F;
	Rectangle SecondScrBorder{
		(float)CasioBaseFrame().x + SecondScrSet.pad - s_pad,
		(float)CasioBaseFrame().y + (SecondScrSet.y - h_pad) + (lightPadUPDOWN / 0.45F),
		(float)CALC_WIDTH - ((SecondScrSet.pad - s_pad - lightPadSide) * 2),
		(float)SecondScrSet.h + (h_pad * 2) - lightPadUPDOWN * 1.80F
	};
	return SecondScrBorder;
}

void Battery(int& time_hours)
{
	Color BatteryColor = GRAY;

	// Draw The Battery Frame
	float left_pad = 10.F;
	float h_pad = 14.F;
	float batteryLength = 30;
	Rectangle BatteryFrame{
		(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad,
		(float)CasioBaseFrame().y + SecondScrSet.y + h_pad,
		(float)batteryLength,
		(float)SecondScrSet.h - (h_pad * 2)
	};
	DrawRectangleRoundedLines(BatteryFrame, 0.1f, 10, 2, BatteryColor);

	// Draw The Battery Anode
	float y_pos_anode = 17.F;
	Rectangle Anode{
		(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad + batteryLength,
		(float)CasioBaseFrame().y + SecondScrSet.y + y_pos_anode,
		(float)5,
		(float)SecondScrSet.h - (y_pos_anode * 2)
	};
	DrawRectangleRounded(Anode, 0.5f, 10, BatteryColor);

	// Draw the cell
	Color cellColor = BatteryColor;
	float alpha = 1.F;

	int cellBattery = 0;
	if (time_hours >= 1 && time_hours < 10) {
		cellBattery = 4;
	}
	else if (time_hours >= 10 && time_hours < 13) {
		cellBattery = 3;
	}
	else if (time_hours >= 13 && time_hours < 17) {
		cellBattery = 2;
	}
	else if (time_hours >= 17 && time_hours < 20) {
		cellBattery = 1;
	}
	else {
		cellBattery = 1;
		cellColor = {200, 70, 70};
		// BLINKING
		double currentTime = GetTime();
		double speedDivider = 1.75;
		double sineValue = sin(currentTime * 2.0 * PI / speedDivider);  // Adjust frequency as needed
		alpha = static_cast<float>((sineValue + 1.0) / 2.0);
	}

	float cellWidth = batteryLength / 5.5F;
	float yCellPad = 15.85F;
	float sidePad = 1.585F;
	for (int i = 0; i < cellBattery; i++) {
		Rectangle Cell{
			(float)CasioBaseFrame().x + SecondScrSet.pad + left_pad + (i * cellWidth) + (i * sidePad) + sidePad,
			(float)CasioBaseFrame().y + SecondScrSet.y + yCellPad,
			(float)cellWidth,
			(float)SecondScrSet.h - (yCellPad * 2)
		};
		DrawRectangleRounded(
			Cell, 
			0.5f, 
			10, 
			Fade(cellColor, alpha)
		);
	}
}

int LiveClock(Font& FontTimeStyle)
{
	Color ClockColor = GRAY;

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

	float font_size = FONT_TIME_SIZE / 4.25F;
	char timeString[9];
	strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeInfo);

	Vector2 TimePos = MeasureTextEx(FontTimeStyle, TextFormat("%s", timeString), font_size, 0);

	Vector2 TimePosDraw{ (float)CALC_WIDTH / 2 - (TimePos.x / 2) + 2.5f, (float)(SecondScrSet.h / 2) + (float)(TimePos.y / 0.725F) };
	DrawTextEx(
		FontTimeStyle,
		TextFormat("%s", timeString),
		TimePosDraw,
		font_size,
		2.5f,
		ClockColor
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

Rectangle MainScrLight()
{
	float sidePad = 0.50F;
	float upDownPad = 0.35F;

	Rectangle MainScrLight{
		(float)CasioBaseFrame().x + MainScrSet.pad + sidePad,
		(float)CasioBaseFrame().y + MainScrSet.y + (upDownPad * 5.25F),
		(float)CALC_WIDTH - ((MainScrSet.pad - sidePad) * 2) + sidePad * 2,
		(float)MainScrSet.h - (upDownPad * 2)
	};

	return MainScrLight;
}

Rectangle MainScrShadow()
{
	float sidePad = 1.6F;
	float upDownPad = 0.75F;

	Rectangle MainScrShadow{
		(float)CasioBaseFrame().x + MainScrSet.pad - sidePad,
		(float)CasioBaseFrame().y + MainScrSet.y - upDownPad,
		(float)CALC_WIDTH - ((MainScrSet.pad - sidePad) * 2),
		(float)MainScrSet.h + (upDownPad * 2.5F)
	};
	return MainScrShadow;

}

Rectangle MainScrBorder() 
{
	float s_pad = 10.F; // sidepad
	float h_pad = 7.F;  // top-bottom pad

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
