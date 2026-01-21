#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <string>
#include <QMainWindow>

#include "../MyShape.h"

// Forward declarations
class GUIPrivate;
class CanvasHelper;
class QGraphicsView;
class QToolBar;
class MouseHandler;
class QLabel;
class QHBoxLayout;
class QDockWidget;
class ColorPicker;
class QPushButton;
class GUIHandler : public QMainWindow{
    public: 
    GUIHandler();
    ~GUIHandler();

    void start();

    // Use for GUI display only
    inline static bool isFilled = false;
    inline static bool isStroked = true;
    inline static double gridLine = 0.0;
    inline static std::string userStrokeColor;
    inline static std::string userFillColor;
    inline static std::string startGradient, endGradient;

    private:
    std::unique_ptr<GUIPrivate> m_guiPrivate;
    std::unique_ptr<CanvasHelper> m_canvas;
    std::unique_ptr<QGraphicsView> m_gv;
    std::unique_ptr<QToolBar> m_toolbar;
    std::unique_ptr<MouseHandler> mouseHandler;
    QLabel* coodinatePreview;
    QHBoxLayout* MainMenu, *shapeButton, *topContainer, *rightContainer;
    QDockWidget* gridsizebox, *lineWidthbox, *colorSeletionbox, *rgbStatusbox, *rgbSeletionbox;
    
    // Buttons
    QPushButton* lineBtn , *triangleBtn, *squareBtn, *rectangleBtn, *circleBtn, *cancelBtn, *backBtn, *drawShapeBtn, *addShapeBtn;
};

#endif // GUIHANDLER_H