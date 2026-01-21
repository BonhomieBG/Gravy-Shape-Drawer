#include <string>
#include <memory>
#include <functional>
#include <QLinearGradient>
#include <QColor>

#include "../DisableCopyMove.h"

class ColorPicker{
    public:
    static std::function<void()> updateCallback;
    static bool useGradient;
    static std::string gradientStartColor;
    static std::string gradientEndColor;
    static std::string gradientDirection; // HORIZONTAL, VERTICAL, DIAGONAL
    static const std::string GDefaultStart;
    static const std::string GDefaultEnd;
    static const std::string GDefaultDirection;
    static int r, g, b;

    DISABLE_COPY_MOVE_DEFAULT(ColorPicker);
    static bool isUseGradient();
    static void setUseGradient(bool option);
    static void setGradientColors(std::string* startColor, std::string* endColor);
    static void setGradientDirection(std::string* direction);

    static std::shared_ptr<std::string> getGradientDirection();
    static std::shared_ptr<std::string> getGradientStartColor();
    static std::shared_ptr<std::string> getGradientEndColor();
    static void getRGB();
    static std::string getColoringMode();

    static void resetGradientColor();
    static QLinearGradient createGradient(std::string* color1, std::string* color2);
    static QLinearGradient createGradient(); // default color version
    static void resetGradientSetting();
    static void setRGB(int r, int g, int b);
    static void resetRGB();
};