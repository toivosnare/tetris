/* TIE-02201/TIE-02207 SPRING 2020
 * Program author
 * Name: Toivo Snåre
 * userID: snare
 * E-Mail: toivo.snare@tuni.fi
 *
 * Class: MainWindow
 * Class that represents a Tetris game with a graphical user interface.
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "tetromino.hh"
#include "pixelview.hh"
#include <QMainWindow>
#include <QVector2D>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <random>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
protected:
	void keyPressEvent(QKeyEvent* event) override;
private:
	Ui::MainWindow* ui;
	PixelView* mainScreen;
	PixelView* nextScreen;
	PixelView* holdScreen;
	QLabel* scoreLabel;
	QLabel* helpLabel;
	QTimer* timer;
	// Cells of tetrominoes that have been already been locked in place.
	QVector<QBrush> field;
	Tetromino* currentTetromino = nullptr;
	Tetromino* nextTetromino = nullptr;
	Tetromino* holdTetromino = nullptr;
	const int WINDOW_HEIGHT = 720;
	const QSize GRID_SIZE = {10, 20};
	const QColor CLEAR_COLOR = {45, 45, 45};
	const QVector2D DISPLAY_POSITION = {2, 2};
	const QVector2D STARTING_POSITION = {5, -1};
	const QVector2D RIGHT = {1, 0};
	const QVector2D DOWN = {0, 1};
	const QVector2D LEFT = {-1, 0};
	const float DIFFICULTY = 0.90;
	const QString HELP_STRING =	"W = ROTATE\n"
								"A = LEFT\n"
								"S = DOWN\n"
								"D = RIGHT\n"
								"Q = DROP\n"
								"E = HOLD";
	const QString GAME_OVER_STRING = "GAME OVER\nPRESS SPACE TO RESTART";
	struct Mold {
		QVector<QVector2D> shape;
		QBrush color;
	};
	const QVector<Mold> molds = {
		{{{0, -1}, {0, 0}, {0, 1}, {0, 2}}, Qt::blue},
		{{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, Qt::yellow},
		{{{-1, 1}, {0, 1}, {0, 0}, {1, 0}}, Qt::red},
		{{{-1, 0}, {0, 0}, {0, 1}, {1, 1}}, Qt::green},
		{{{0, -1}, {0, 0}, {0, 1}, {1, 1}}, Qt::darkYellow},
		{{{0, -1}, {0, 0}, {0, 1}, {-1, 1}}, Qt::magenta},
		{{{-1, 0}, {0, 0}, {1, 0}, {0, 1}}, Qt::darkMagenta}
	};
	std::default_random_engine randomEng;
	std::uniform_int_distribution<int> distr;
	int score = 0;
	bool canHold = true;
	bool gameOver = false;

	// Resets the game by setting all member variables to their initial values.
	void reset();
	// Stops the game from advancing until new game is started.
	void stop();
	// Moves current tetromino downwards. If not succesful, locks it in place.
	// Finally updates the screen.
	void tick();
	// Displays the current tetromino and the playing field on the main screen.
	void render();
	// Moves the curent tetromino in the direction specified by deltaPos and
	// deltaRot. Returns true if the movement could be made.
    bool move(const QVector2D& deltaPos, int deltaRot = 0);
	// Gets the previously randomly drawn tetromino to the starting position and
	// draws the next tetromimo ready for the next round.
	void next();
	// If the canHold flag is set, switches places of the current tetromino and
	// the tetromino in the hold slot.
	void hold();
	// Moves the current tetromino downwards until it cannot be moved anymore.
	// Then locks it in place.
	void drop();
	// Stores the cells of the current tetromino in the playing field. Stops
	// the game if at least one of the cells are above the playing field.
	// Checks if any rows get filled up and updates the score accordinly.
	// Finally gets the next tetromino by calling next().
	void lockInPlace();
	// Moves rows above the row to be removed downwards.
    void removeRow(int row);
	// Returns true if the current cell does not go out of bounds or overlap
	// with the playing field when moving to newPos and newRot.
    bool canMoveTo(const QVector2D& newPos, int newRot) const;
	// Returns true if a row of the playing field specified by the row
	// parameter is completely full.
    bool isRowFull(int row) const;
	// Return true if a cell of the playing field specified by the parameters
	// is empty.
    bool isCellEmpty(int x, int y) const;
    bool isCellEmpty(int i) const;
};

#endif // MAINWINDOW_HH
