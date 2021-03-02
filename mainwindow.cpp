#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <set>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	// Setup GUI elements.
	mainScreen = new PixelView(WINDOW_HEIGHT, GRID_SIZE, this);
	nextScreen = new PixelView(6 * mainScreen->cellSize(), QSize(5, 6), this);
	holdScreen = new PixelView(6 * mainScreen->cellSize(), QSize(5, 6), this);

	holdScreen->move(mainScreen->cellSize(), mainScreen->cellSize());
	mainScreen->move(holdScreen->geometry().right() + mainScreen->cellSize(), 0);
	nextScreen->move(mainScreen->geometry().right() + mainScreen->cellSize(), mainScreen->cellSize());
	setFixedSize(nextScreen->geometry().right() + mainScreen->cellSize(), WINDOW_HEIGHT);

	QLabel* holdLabel = new QLabel("HOLD", this);
	holdLabel->move(holdScreen->pos() + QPoint(0, -30));
	QLabel* nextLabel = new QLabel("NEXT", this);
	nextLabel->move(nextScreen->pos() + QPoint(0, -30));
	QLabel* scoreTitleLabel = new QLabel("SCORE", this);
	scoreTitleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	scoreTitleLabel->move(holdScreen->geometry().bottomLeft() + QPoint(0, 10));;
	scoreLabel = new QLabel("0", this);
	scoreLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	scoreLabel->move(scoreTitleLabel->geometry().topRight());
	helpLabel = new QLabel(HELP_STRING, this);
	helpLabel->setWordWrap(true);
	helpLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	helpLabel->setMinimumSize(nextScreen->width(), 150);
	helpLabel->move(nextScreen->geometry().bottomLeft() + QPoint(0, 10));

	// Intialize RNG
	randomEng.seed(time(0));
	distr = std::uniform_int_distribution<int>(0, molds.size() - 1);
	distr(randomEng);

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainWindow::tick);

	field.resize(GRID_SIZE.width() * GRID_SIZE.height());
	reset();
}

MainWindow::~MainWindow() {
	delete ui;
	delete currentTetromino;
	delete nextTetromino;
	if(holdTetromino != nullptr)
		delete holdTetromino;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
	if(gameOver) {
		if(event->key() == Qt::Key_Space)
			reset();
		return;
	}

	if(event->key() == Qt::Key_D)
		move(RIGHT);
	else if(event->key() == Qt::Key_A)
		move(LEFT);
	else if(event->key() == Qt::Key_S)
		move(DOWN);
	else if(event->key() == Qt::Key_W)
		move({0, 0}, 1);
	else if(event->key() == Qt::Key_E)
		hold();
	else if(event->key() == Qt::Key_Q)
		drop();
	else
		return;

	render();
}

void MainWindow::reset() {
	if(currentTetromino != nullptr)
		delete currentTetromino;
	if(nextTetromino != nullptr)
		delete nextTetromino;
	if(holdTetromino != nullptr)
		delete holdTetromino;
	timer->setInterval(1000);
	score = 0;
	scoreLabel->setText(QString::number(score));
	helpLabel->setText(HELP_STRING);
	holdTetromino = nullptr;
	canHold = true;
	gameOver = false;
	mainScreen->clear(CLEAR_COLOR);
	nextScreen->clear(CLEAR_COLOR);
	holdScreen->clear(CLEAR_COLOR);
	for(int i = 0; i < field.size(); ++i)
		field[i] = CLEAR_COLOR;
	Mold mold = molds[distr(randomEng)];
	nextTetromino = new Tetromino(mold.shape, DISPLAY_POSITION, mold.color);
	timer->start();
	next();
}

void MainWindow::stop() {
	gameOver = true;
	helpLabel->setText(GAME_OVER_STRING);
	timer->stop();
}

void MainWindow::tick() {
	if(!move(DOWN))
		lockInPlace();
	render();
}

void MainWindow::render() {
	mainScreen->clear(CLEAR_COLOR);
	currentTetromino->render(mainScreen);
	for(int i = 0; i < field.size(); ++i) {
		if(!isCellEmpty(i))
			mainScreen->setPixel(QVector2D(i % GRID_SIZE.width(), i / GRID_SIZE.width()), field[i]);
	}
}

bool MainWindow::move(const QVector2D& deltaPos, int deltaRot) {
	QVector2D newPos = currentTetromino->getPosition() + deltaPos;
	int newRot = (currentTetromino->getRotation() + deltaRot) % 4;
	if(canMoveTo(newPos, newRot)) {
		currentTetromino->setPosition(newPos);
		currentTetromino->setRotation(newRot);
		return true;
	}
	return false;
}

void MainWindow::next() {
	currentTetromino = nextTetromino;
	currentTetromino->setPosition(STARTING_POSITION);
	// Draw next tetromino ready.
	Mold mold = molds[distr(randomEng)];
	nextTetromino = new Tetromino(mold.shape, DISPLAY_POSITION, mold.color);
	nextScreen->clear(CLEAR_COLOR);
	// Display the next tetromino on the nextScreen.
	nextTetromino->render(nextScreen);
	canHold = true;
}

void MainWindow::hold() {
	if(!canHold) return;
	if(holdTetromino == nullptr) {
		holdTetromino = currentTetromino;
		holdTetromino->setPosition(DISPLAY_POSITION);
		// Display the tetromino in the hold slot on the holdScreen.
		holdTetromino->render(holdScreen);
		next();
	} else {
		delete currentTetromino;
		currentTetromino = holdTetromino;
		holdTetromino = nullptr;
		holdScreen->clear(CLEAR_COLOR);
		currentTetromino->setPosition(STARTING_POSITION);
		canHold = false;
	}
}

void MainWindow::drop() {
	while(move(DOWN));
	lockInPlace();
}

void MainWindow::lockInPlace() {
	std::set<int> rowsToCheck;
	for(QVector2D& cell : currentTetromino->cells()) {
		if(cell.y() < 0) {
			stop();
			return;
		}
		// Store the cell in the playing field.
		field[cell.y() * GRID_SIZE.width() + cell.x()] = currentTetromino->getColor();
		rowsToCheck.insert(cell.y());
	}
	delete currentTetromino;
	for(int row : rowsToCheck) {
		if(isRowFull(row)) {
			removeRow(row);
			score++;
			scoreLabel->setText(QString::number(score));
			timer->setInterval(timer->interval() * DIFFICULTY);
		}
	}
	next();
}

void MainWindow::removeRow(int row) {
	// Start from the last cell of the row above the row to be removed.
	int start = row * GRID_SIZE.width() - 1;
	for(int i = start; i >= 0; --i) {
		// Move the cell one row forward.
		field.move(i, i + GRID_SIZE.width());
		field[i] = CLEAR_COLOR;
	}
	// Clear the first row.
	for(int x = 0; x < GRID_SIZE.width(); ++x)
		field[x] = CLEAR_COLOR;
}

bool MainWindow::canMoveTo(const QVector2D& newPos, int newRot) const {
	for(QVector2D& cell : currentTetromino->cells(newPos, newRot)) {
		// Check if the cell is out of bounds on the x-axis.
		if(cell.x() < 0 || cell.x() >= GRID_SIZE.width())
			return false;
		// Skip cells that are in the negative y-coordinates because the
		// tetromino starting position is above the playing field.
		if(cell.y() < 0)
			continue;
		// Check if the cell is under the playing field.
		if(cell.y() >= GRID_SIZE.height())
			return false;
		// Check if the cell overlaps with a cell in the playing field.
		if(field[cell.y() * GRID_SIZE.width() + cell.x()] != CLEAR_COLOR)
			return false;
	}
	return true;
}

bool MainWindow::isRowFull(int row) const {
	for(int x = 0; x < GRID_SIZE.width(); ++x) {
		if(isCellEmpty(x, row))
			return false;
	}
	return true;
}

bool MainWindow::isCellEmpty(int x, int y) const {
	// Conversion from two dimensional coordinates to one dimensional data
	// structure.
	return isCellEmpty(y * GRID_SIZE.width() + x);
}

bool MainWindow::isCellEmpty(int i) const {
	return field[i] == CLEAR_COLOR;
}
