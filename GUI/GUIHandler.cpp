// Set up stage for display
// GUIHandler handle the main window to display all elements of the program. Linking mouse handler, gradient color, 
// RGB and canvas helper to the main window for interaction.

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>
#include <QMainWindow>
#include <vector>
#include <QInputDialog>
#include <QToolBar>
#include <QAction>
#include <string>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <functional>
#include <QGraphicsItem>
#include <QPushButton>
#include <QDockWidget>
#include <type_traits>
#include <QStackedLayout>
#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "CanvasHelper.h"
#include "GUIHandler.h"
#include "MouseHandler.h"
#include "ColorPicker.h"
#include "../Multithread.hpp"
#include "../Utility.hpp"
#include "../MyLine.h"
#include "../MyCircle.h"
#include "../MySquare.h"
#include "../MyRectangle.h"
#include "../MyTriangle.h"
#include "../MyPoint.h"

class GUIPrivate{
    private:
    GUIHandler* gui;
    CanvasHelper* canvas;
    std::vector<QGraphicsLineItem*> gridItems;
    public:

    GUIPrivate(GUIHandler* guiHandler, CanvasHelper* canvasHelper) 
        : gui(guiHandler), canvas(canvasHelper) {
        propertyTitle->setStyleSheet("font-weight: bold; font-size: 14px; padding: 5px;");
        
        // Initialize labels with proper values now that gui and canvas are set
        strokeColorLabel->setText(QString::fromStdString("Outline: " + gui->userStrokeColor));
        fillColorLabel->setText(QString::fromStdString("Filled: " + std::string(gui->isFilled ? "Yes" : "No")));
        lineWidthLabel->setText(QString::fromStdString("Line Width: "+ std::string(gui->gridLine == 0.0? "Default" : std::to_string (gui->gridLine))));
        gradientStatusLabel->setText(QString::fromStdString("Gradient: "+ std::string(ColorPicker::isUseGradient()? "Yes": "No")));
        shapeCountLabel->setText(QString::fromStdString("Total Shapes: "+ std::to_string(canvas->getAllShapes().size())));
        gradientColorLabel->setText(QString::fromStdString("Gradient Colors: "+ std::string(
            !gui->startGradient.empty() && !gui->endGradient.empty()? gui->startGradient + " to " + gui->endGradient : "Default")));
    };

    ~GUIPrivate() = default;

    // Checkerboard custom size (scene, width, height, gridsize)
    void CheckerGrid(QGraphicsScene* scene, double width, double height, int gridsize){
        QPen gridPen(Qt::lightGray);
        gridPen.setWidthF(1.0);

        // Clear existing grid first
        clearGrid(scene);

        // Draw vertical lines
        for (int x = 0; x < width; x += gridsize){
            QGraphicsLineItem* line = scene->addLine(x, 0, x, height, gridPen);
            line->setZValue(-1000);  // Place grid behind all other items
            line->setFlag(QGraphicsItem::ItemIsSelectable, false);
            line->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            gridItems.push_back(line);
        }
        
        // Draw horizontal lines
        for (int y = 0; y < height; y += gridsize){
            QGraphicsLineItem* line = scene->addLine(0, y, width, y, gridPen);
            line->setZValue(-1000);  // Place grid behind all other items
            line->setFlag(QGraphicsItem::ItemIsSelectable, false);
            line->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            gridItems.push_back(line);
        }
    };
    
    // checker board default(graphic-scene)
    void CheckerGridDefault(QGraphicsScene* scene){
        QPen gridPen(Qt::lightGray);
        gridPen.setWidthF(1.0);

        // Clear existing grid first
        clearGrid(scene);

        QRectF rect = scene->sceneRect();
        int gridsize = 20;
        for (int x = 0; x < rect.width(); x += gridsize){
            QGraphicsLineItem* line = scene->addLine(x, 0, x, rect.height(), gridPen);
            line->setZValue(-1000);
            line->setFlag(QGraphicsItem::ItemIsSelectable, false);
            line->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            gridItems.push_back(line);
        }
        for (int y = 0; y < rect.height(); y += gridsize){
            QGraphicsLineItem* line = scene->addLine(0, y, rect.width(), y, gridPen);
            line->setZValue(-1000);
            line->setFlag(QGraphicsItem::ItemIsSelectable, false);
            line->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            gridItems.push_back(line);
        }
    };
    
    void clearGrid(QGraphicsScene* gs){
        for (auto item : gridItems){
            gs->removeItem(item);
            delete item;
        }
        gridItems.clear();
    };

    // use 2D array for realitic color tracking
    inline static std::string colors[4][4] = {
            {"black", "white", "gray", "lightgray"},
            {"red", "green", "blue", "yellow"},
            {"orange", "purple", "pink", "cyan"},
            {"brown", "lime", "navy", "magenta"}
    };

    QLabel* propertyTitle = new QLabel ("Current Settings");

    QLabel* strokeColorLabel = new QLabel();
    QLabel* fillColorLabel = new QLabel();
    QLabel* lineWidthLabel = new QLabel();
    QLabel* gradientStatusLabel = new QLabel();
    QLabel* shapeCountLabel = new QLabel();
    QLabel* gradientColorLabel = new QLabel();

    std::function<void()> updatePropertyPanel = [this](){
        strokeColorLabel->setText(QString::fromStdString("Outline: " + gui->userStrokeColor));
        fillColorLabel->setText(QString::fromStdString("Filled: " + std::string(gui->isFilled ? "Yes" : "No")));
        lineWidthLabel->setText(QString::fromStdString("Line Width: "+ std::string(gui->gridLine == 0.0? "Default" : std::to_string (gui->gridLine))));
        gradientStatusLabel->setText(QString::fromStdString("Gradient: "+ std::string(ColorPicker::isUseGradient()? "Yes": "No")));
        shapeCountLabel->setText(QString::fromStdString("Total Shapes: "+ std::to_string(canvas->getAllShapes().size())));
        gradientColorLabel->setText(QString::fromStdString("Gradient Colors: "+ std::string(
            !gui->startGradient.empty() && !gui->endGradient.empty()? gui->startGradient + " to " + gui->endGradient : "Default")));
    };
};

GUIHandler::~GUIHandler() = default;

GUIHandler::GUIHandler(){
    this->coodinatePreview = new QLabel(this);
    this->m_gv = std::make_unique<QGraphicsView>();
    this->m_canvas = std::make_unique<CanvasHelper>(m_gv.get());
    this->m_guiPrivate = std::make_unique<GUIPrivate>(this, m_canvas.get());
    this->m_toolbar = std::make_unique<QToolBar>();
    this->m_canvas->setGUIHandler(this);
    this->mouseHandler = std::make_unique<MouseHandler> (m_canvas.get(), coodinatePreview, m_gv.get()); // mousehandler is qgraphicscene subclass
    this->MainMenu = new QHBoxLayout();
    this->topContainer = new QHBoxLayout();
    this->shapeButton = new QHBoxLayout();
    this->rightContainer = new QHBoxLayout();
    this->gridsizebox = new QDockWidget();
    this->lineWidthbox = new QDockWidget();
    this->colorSeletionbox = new QDockWidget();
    this->rgbSeletionbox = new QDockWidget();
    this->rgbStatusbox = new QDockWidget();
};

void GUIHandler::start(){
    // create graphic view and scene
    m_gv->setScene(mouseHandler.get());

    // Disable scrolling
    m_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Install mouse event handler on the graphics view
    m_gv->installEventFilter(mouseHandler.get());
    m_gv->setMouseTracking(true);  // Enable mouse tracking even without buttons pressed

    // Create menu bar with dropdown menus
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    // Make sure menu bar has the same color throughout the application
    menuBar->setStyleSheet(
        "QMenuBar { background-color: #f0f0f0; }"
        "QMenuBar::item { padding: 2px 10px; }"
        "QMenuBar::item:selected { background-color: #d0d0d0; }"
    );

    // File Menu

    QMenu* fileMenu = menuBar->addMenu("File");
    QAction* newDrawingAction = fileMenu->addAction("New Drawing");
    QAction* saveAction = fileMenu->addAction("Save");
    QAction* loadAction = fileMenu->addAction("Load");
    QAction* exitAction = fileMenu->addAction("Exit");

    // Edit Menu
    QMenu* editMenu = menuBar->addMenu("Edit");
    QAction* undoAction = editMenu->addAction("Undo");
    QAction* redoAction = editMenu->addAction("Redo");
    QAction* clearSettingAction = editMenu->addAction("Clear Setting");

    // Options Menu
    QMenu* optionsMenu = menuBar->addMenu("Options");
    QAction* setBgColorAction = optionsMenu->addAction("Set Background Color");
    QAction* enableGradientAction = optionsMenu->addAction("Enable Gradient Colors");
    QAction* enableFilledAction = optionsMenu->addAction("Enable Color Filling");
    enableFilledAction->setCheckable(true);
    enableGradientAction->setCheckable(true);
    QAction* setGradientColorAction = optionsMenu->addAction("Set Gradient Color");
    QAction* setGradientDirectionAction = optionsMenu->addAction("Set Gradient Direction");

    // Help Menu
    QMenu* helpMenu = menuBar->addMenu("Help");
    QAction* instructionsAction = helpMenu->addAction("Instructions");
    QAction* aboutAction = helpMenu->addAction("About");

    // Button for Auto Drawing
    lineBtn = new QPushButton("Draw Line", this);
    triangleBtn = new QPushButton("Draw Triangle", this);
    squareBtn = new QPushButton("Draw Square", this);
    rectangleBtn = new QPushButton("Draw Rectangle", this);
    circleBtn = new QPushButton("Draw Circle", this);
    cancelBtn = new QPushButton("Cancel Drawing", this);
    backBtn = new QPushButton("Back to MainMenu", this);
    drawShapeBtn = new QPushButton("Draw Shape", this);
    addShapeBtn = new QPushButton("Add Shape Manually", this);
    
    // Modern button stylesheet
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}";
    
    QString cancelStyle = 
        "QPushButton {"
        "    background-color: #f44336;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #da190b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #c1160a;"
        "}";
    
    QString backStyle = 
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px 16px;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "    min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0b7dda;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0a6ebd;"
        "}";
    
    lineBtn->setStyleSheet(buttonStyle);
    triangleBtn->setStyleSheet(buttonStyle);
    squareBtn->setStyleSheet(buttonStyle);
    rectangleBtn->setStyleSheet(buttonStyle);
    circleBtn->setStyleSheet(buttonStyle);
    drawShapeBtn->setStyleSheet(buttonStyle);
    addShapeBtn->setStyleSheet(buttonStyle);
    cancelBtn->setStyleSheet(cancelStyle);
    backBtn->setStyleSheet(backStyle);
    
    MainMenu->addWidget(addShapeBtn);
    MainMenu->addWidget(drawShapeBtn);
    
    shapeButton->addWidget(lineBtn);
    shapeButton->addWidget(triangleBtn);
    shapeButton->addWidget(squareBtn);
    shapeButton->addWidget(rectangleBtn);
    shapeButton->addWidget(circleBtn);
    shapeButton->addWidget(cancelBtn);
    shapeButton->addWidget(backBtn);
    
    // Initially hide shape buttons
    lineBtn->hide();
    triangleBtn->hide();
    squareBtn->hide();
    rectangleBtn->hide();
    circleBtn->hide();
    cancelBtn->hide();
    backBtn->hide();
    
    // Add both layouts to topContainer
    topContainer->addLayout(MainMenu);
    topContainer->addLayout(shapeButton);
    
    // Create a widget to hold the topContainer layout and add it to toolbar
    QWidget* topWidget = new QWidget(this);
    topWidget->setLayout(topContainer);
    m_toolbar->addWidget(topWidget);

    // Manually drawing
    connect(addShapeBtn, &QPushButton::clicked, [this](){
        enum ShapeType {
            LINE,
            CIRCLE,
            RECTANGLE,
            SQUARE,
            TRIANGLE
        };

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Add Shape Manually");
        msgBox.setText("Choose Shape Type:");
        
        // Create buttons and store enum values in their data
        QPushButton* lineBtn = msgBox.addButton("Line", QMessageBox::ActionRole);
        lineBtn->setProperty("shapeType", LINE);
        
        QPushButton* circleBtn = msgBox.addButton("Circle", QMessageBox::ActionRole);
        circleBtn->setProperty("shapeType", CIRCLE);
        
        QPushButton* rectangleBtn = msgBox.addButton("Rectangle", QMessageBox::ActionRole);
        rectangleBtn->setProperty("shapeType", RECTANGLE);
        
        QPushButton* squareBtn = msgBox.addButton("Square", QMessageBox::ActionRole);
        squareBtn->setProperty("shapeType", SQUARE);
        
        QPushButton* triangleBtn = msgBox.addButton("Triangle", QMessageBox::ActionRole);
        triangleBtn->setProperty("shapeType", TRIANGLE);
        
        msgBox.addButton(QMessageBox::Cancel);
        
        msgBox.exec();
        
        // Get the clicked button and retrieve its enum value
        if (msgBox.clickedButton() && msgBox.clickedButton() != msgBox.button(QMessageBox::Cancel)) {
            QVariant shapeData = msgBox.clickedButton()->property("shapeType");
            if (shapeData.isValid()) {
                try{
                    ShapeType selectedShape = static_cast<ShapeType>(shapeData.toInt());
                
                // Use switch statement with enum
                switch (selectedShape) {
                    case LINE:
                        m_canvas->addLine();
                        break;
                    case CIRCLE:
                        m_canvas->addCircle();
                        break;
                    case RECTANGLE:
                        m_canvas->addRectangle();
                        break;
                    case SQUARE:
                        m_canvas->addSquare();
                        break;
                    case TRIANGLE:
                        m_canvas->addTriangle();
                        break;
                }
                } catch (std::exception& e){ 
                    showErrorAlert("Error", e.what());
                }
            }
        }
    });

    // Auto drawing
    connect(drawShapeBtn, &QPushButton::clicked, [this](){
        // Hide MainMenu widgets
        addShapeBtn->hide();
        drawShapeBtn->hide();
        
        // Show shapeButton widgets
        lineBtn->show();
        triangleBtn->show();
        squareBtn->show();
        rectangleBtn->show();
        circleBtn->show();
        cancelBtn->show();
        backBtn->show();
    });

    connect(lineBtn, &QPushButton::clicked, [this](){
        mouseHandler->setShapeType(std::string("LINE"));
    });

    connect(triangleBtn, &QPushButton::clicked, [this](){
        mouseHandler->setShapeType(std::string("Triangle"));
    });

    connect(squareBtn, &QPushButton::clicked, [this](){
        mouseHandler->setShapeType(std::string("Square"));
    });

    connect(rectangleBtn, &QPushButton::clicked, [this](){
        mouseHandler->setShapeType(std::string("Rectangle"));
    });

    connect(circleBtn, &QPushButton::clicked, [this](){
        mouseHandler->setShapeType(std::string("Circle"));
    });

    connect(cancelBtn, &QPushButton::clicked, [this](){
        mouseHandler->cancelCurrentDrawing();
    });

    connect(backBtn, &QPushButton::clicked, [this](){
        // Hide shapeButton widgets
        lineBtn->hide();
        triangleBtn->hide();
        squareBtn->hide();
        rectangleBtn->hide();
        circleBtn->hide();
        cancelBtn->hide();
        backBtn->hide();
        
        // Show MainMenu widgets
        addShapeBtn->show();
        drawShapeBtn->show();
        
        mouseHandler->cancelCurrentDrawing();
    });

    // Connect menu actions to slots
    connect(newDrawingAction, &QAction::triggered, [this]() {
        // Clear all shapes
        m_canvas->clearCanvas(m_gv.get());
        m_guiPrivate->updatePropertyPanel();
    });

    // Save
    connect(saveAction, &QAction::triggered, [this]() {
        
    });

    // Load
    connect(loadAction, &QAction::triggered, [this]() {
        
    });


    connect(exitAction, &QAction::triggered, [this]() {
        QApplication::quit();
    });

    connect(undoAction, &QAction::triggered, [this](){
        m_canvas->undoShape();
        m_guiPrivate->updatePropertyPanel();
    });

    connect(redoAction, &QAction::triggered, [this](){
        m_canvas->redoShape();
        m_guiPrivate->updatePropertyPanel();
    });

    // using lambda to get conformation from user
    connect(clearSettingAction, &QAction::triggered, [this](){
        showWarningCustom("Clear Settings?", "Are you sure you want to reset all settings to default?", "Yes", "No", [this](bool result){
            if (result){
                // Reset all color and gradient settings
                ColorPicker::resetGradientColor();
                ColorPicker::resetGradientSetting();
                ColorPicker::gradientDirection = ColorPicker::GDefaultDirection;
                ColorPicker::resetRGB();
                          
                // Reset background to transparent
                mouseHandler->setBackgroundBrush(QBrush(Qt::transparent));
                m_gv->setBackgroundBrush(QBrush(Qt::transparent));
                
                showInfoAlert("Settings Reset", "All settings have been reset to defaults.");
                m_guiPrivate->updatePropertyPanel();
            }
        });
    });

    connect(instructionsAction, &QAction::triggered, [this]() {
        QMessageBox::information(this, "Instructions",
        R"==(
            ==== How to Use Shape Management GUI ====

                    DRAWING SHAPES
        1. Click on shape button (Line, Circle, Triangle, etc...)

        2. Click on canvas to place points: 
            - Line: 2 points (start, end)
            - Circle/Square/Rectangle: 2 points 
            (top left, buttom right or special attribute and top left)
            - Triangle: 3 pointss 
            (each starting points or each triangle side length)

        3. Shape is automatically created when all points are placed.

                    MENU OPTION
        • File -> New Drawing: Clear all Shapes
        • File -> Exit: Close application
        • File -> Save: Save drawing to a file
        • File -> Load: Load drawing from file
        • Edit -> Undo: Remove the last done shape
        • Edit -> Redo: Restore last removed shape
        • Edit -> Clear Setting: Restore default setting
        • Option -> Set Background Color = Change Canvas Color
        • Option -> Enable Gradient = Toggle gradient color
        • Option -> Set Gradient Color = Set both start and end gradient color
        • Option -> Set Gradient Direction = Set direct of gradient coloring

                    TIP:
        • Coodinate shows current mouse's position 
        and reveal X and Y coodinate value automatically during hovering
        • Cancel button stop current drawing operation
        • All shape default to black for stroke and transparent if no color was seleted
        • Quick Colors can be found on the right side of the application
    )=="
        );
    });

    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::information(this, "About", "Shape Management GUI\nVersion 1.0\n\nDrawing application for managing geometric shapes by BG.");
    });

    // Connect coodinate preview with mouse handler
    coodinatePreview->setStyleSheet(
        "font-size: 11px;"
        "font-family: 'Segoe UI', Arial, sans-serif;"
        "background-color: #f8f9fa;"
        "color: #212529;"
        "padding: 8px;"
        "border: 1px solid #dee2e6;"
        "border-radius: 4px;"
    );

    // Create dock widget for coordinate display (top-left area)
    QDockWidget* coordinateDock = new QDockWidget("Mouse Coordinates", this);
    coordinateDock->setWidget(coodinatePreview);
    coordinateDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // Make it fixed
    addDockWidget(Qt::TopDockWidgetArea, coordinateDock);
    
    // Modern dock widget stylesheet
    QString dockStyle = 
        "QDockWidget {"
        "    titlebar-close-icon: url(close.png);"
        "    titlebar-normal-icon: url(undock.png);"
        "    font-weight: bold;"
        "}"
        "QDockWidget::title {"
        "    background-color: #2c3e50;"
        "    color: white;"
        "    padding: 6px;"
        "    border: 1px solid #34495e;"
        "}"
        "QDockWidget::close-button, QDockWidget::float-button {"
        "    background: transparent;"
        "    border: none;"
        "}";
    
    coordinateDock->setStyleSheet(dockStyle);

    // set central widget and window property
    setCentralWidget(m_gv.get());
    
    // create toolbar for shape buttons
    addToolBar(m_toolbar.get());
    m_toolbar->setMovable(false);
    m_toolbar->setStyleSheet(
        "QToolBar {"
        "    background-color: #ecf0f1;"
        "    border: 1px solid #bdc3c7;"
        "    padding: 4px;"
        "    spacing: 8px;"
        "}"
    );
    
    // Add right side dock widgets
    colorSeletionbox->setWindowTitle("Color Selection");
    colorSeletionbox->setFeatures(QDockWidget::NoDockWidgetFeatures);
    colorSeletionbox->setStyleSheet(dockStyle);
    addDockWidget(Qt::RightDockWidgetArea, colorSeletionbox);
    
    gridsizebox->setWindowTitle("Grid Size");
    gridsizebox->setFeatures(QDockWidget::NoDockWidgetFeatures);
    gridsizebox->setStyleSheet(dockStyle);
    addDockWidget(Qt::RightDockWidgetArea, gridsizebox);

    rgbStatusbox->setWindowTitle("RGB Status");
    rgbStatusbox->setFeatures(QDockWidget::NoDockWidgetFeatures);
    rgbStatusbox->setStyleSheet(dockStyle);
    addDockWidget(Qt::LeftDockWidgetArea, rgbStatusbox);

    rgbSeletionbox->setWindowTitle("RGB Value");
    rgbSeletionbox->setFeatures(QDockWidget::NoDockWidgetFeatures);
    rgbSeletionbox->setStyleSheet(dockStyle);
    addDockWidget(Qt::LeftDockWidgetArea, rgbSeletionbox);
    
    // Apply main window styling
    setStyleSheet(
        "QMainWindow {"
        "    background-color: #ecf0f1;"
        "}"
        "QGraphicsView {"
        "    background-color: white;"
        "    border: 2px solid #95a5a6;"
        "}"
    );
    
    setWindowTitle("Shape Management");
    showMaximized();  // Show maximized instead of fullscreen for better usability
    
    // Process events to ensure window is fully resized before getting viewport size
    QApplication::processEvents();
    QStackedLayout* stack = new QStackedLayout (this);
    
    // Set scene rect to match the viewport size after window is shown
    QSize viewSize = m_gv->viewport()->size();
    mouseHandler->setSceneRect(0, 0, viewSize.width(), viewSize.height());
    
    // Draw grid using GUIPrivate CheckerGrid method
    m_guiPrivate->CheckerGrid(mouseHandler.get(), viewSize.width(), viewSize.height(), 20);
}
