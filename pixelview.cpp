#include "pixelview.hh"
#include <QGraphicsRectItem>

PixelView::PixelView(int height, const QSize& gridSize, QWidget* parent) :
	QGraphicsView(parent), gridSize(gridSize) {
	int width = gridSize.width() * height / gridSize.height();
	setFixedSize(width, height);
	setFrameStyle(0);

	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setSceneRect(0, 0, width, height);
	setScene(scene);

	grid.resize(gridSize.height());
	for(int y = 0; y < gridSize.height(); ++y) {
		grid[y].resize(gridSize.width());
		for(int x = 0; x < gridSize.width(); ++x) {
			QGraphicsRectItem* cell = new QGraphicsRectItem;
			cell->setRect(x * cellSize(), y * cellSize(), cellSize(), cellSize());
			cell->setPen(QPen(Qt::black));
			grid[y][x] = cell;
			scene->addItem(cell);
		}
	}
}

void PixelView::clear(const QBrush& color) {
	for(auto& row : grid)
		for(auto& cell : row)
			cell->setBrush(color);
}

void PixelView::setPixel(const QVector2D& pos, const QBrush& color) {
	// Check if the position is out of bounds.
	if(pos.x() < 0 || pos.x() >= gridSize.width())
		return;
	if(pos.y() < 0 || pos.y() >= gridSize.height())
		return;
	grid[pos.y()][pos.x()]->setBrush(color);
}

int PixelView::cellSize() const {
	return height() / gridSize.height();
}
