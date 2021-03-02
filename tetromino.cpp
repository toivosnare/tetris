#include "tetromino.hh"

Tetromino::Tetromino(const QVector<QVector2D>& shape,
					 const QVector2D& position, const QBrush& color) :
	shape(shape), position(position), color(color), rotation(0) {}

void Tetromino::render(PixelView* screen) const {
	for(QVector2D& pos : cells())
		screen->setPixel(pos, color);
}

QVector<QVector2D> Tetromino::cells() const {
	// Use current position and rotation.
	return cells(position, rotation);
}

QVector<QVector2D> Tetromino::cells(const QVector2D& pos, int rot) const {
	QVector<QVector2D> result;
	for(const QVector2D& offset : shape) {
		int x = 0;
		int y = 0;
		switch (rot) {
			case 0:
				x = pos.x() + offset.x();
				y = pos.y() + offset.y();
				break;
			case 1:
				x = pos.x() - offset.y();
				y = pos.y() + offset.x();
				break;
			case 2:
				x = pos.x() - offset.x();
				y = pos.y() - offset.y();
				break;
			case 3:
				x = pos.x() + offset.y();
				y = pos.y() - offset.x();
				break;
			default:
				x = 0;
				y = 0;
				break;
		}
		result.push_back(QVector2D(x, y));
	}
	return result;
}

QVector2D Tetromino::getPosition() const {
	return position;
}

int Tetromino::getRotation() const {
	return rotation;
}

QBrush Tetromino::getColor() const {
	return color;
}

void Tetromino::setPosition(const QVector2D& newPosition) {
	position = newPosition;
}

void Tetromino::setRotation(int newRotation) {
	rotation = newRotation;
}
