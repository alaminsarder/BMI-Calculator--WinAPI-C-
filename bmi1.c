#include <windows.h>
#include <stdio.h>

#define ID_WEIGHT 1
#define ID_HEIGHT 2
#define ID_CALCULATE 3

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hWeight, hHeight, hButton;

    switch (msg) {
        case WM_CREATE:
            CreateWindow("STATIC", "Weight (kg):", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hwnd, NULL, NULL, NULL);
            hWeight = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 20, 100, 20, hwnd, (HMENU)ID_WEIGHT, NULL, NULL);

            CreateWindow("STATIC", "Height (m):", WS_VISIBLE | WS_CHILD, 20, 60, 100, 20, hwnd, NULL, NULL, NULL);
            hHeight = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 60, 100, 20, hwnd, (HMENU)ID_HEIGHT, NULL, NULL);

            hButton = CreateWindow("BUTTON", "Calculate BMI", WS_VISIBLE | WS_CHILD, 80, 100, 120, 30, hwnd, (HMENU)ID_CALCULATE, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_CALCULATE) {
                char weightStr[20], heightStr[20];
                GetWindowText(hWeight, weightStr, 20);
                GetWindowText(hHeight, heightStr, 20);

                float weight = atof(weightStr);
                float height = atof(heightStr);

                if (height <= 0 || weight <= 0) {
                    MessageBox(hwnd, "Please enter valid positive values!", "Error", MB_OK | MB_ICONERROR);
                } else {
                    float bmi = weight / (height * height);
                    char category[50];

                    if (bmi < 18.5)
                        strcpy(category, "Underweight");
                    else if (bmi < 25)
                        strcpy(category, "Normal weight");
                    else if (bmi < 30)
                        strcpy(category, "Overweight");
                    else
                        strcpy(category, "Obese");

                    char result[200];
                    sprintf(result, "Your BMI is: %.2f\nCategory: %s", bmi, category);
                    MessageBox(hwnd, result, "BMI Result", MB_OK | MB_ICONINFORMATION);
                }
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "BMIApp";
    wc.lpfnWndProc = WindowProc;

    if (!RegisterClass(&wc)) return -1;

    CreateWindow("BMIApp", "BMI Calculator", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 500, 300, 300, 200, NULL, NULL, hInst, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
