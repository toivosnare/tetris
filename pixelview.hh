/* TIE-02201/TIE-02207 SPRING 2020
 * Program author
 * Name: Toivo Snåre
 * userID: snare
 * E-Mail: toivo.snare@tuni.fi
 *
 * Class: PixelView
 * Class that represents a grid of large pixels.
 */

#ifndef PIXELVIEW_HH
#define PIXELVIEW_HH

#include <QGraphicsView>
#include <QVector2D>

class PixelView : public QGraphicsView {
public:
    PixelView(int height, const QSize& gridSize, QWidget* parent);
	// Sets all cells to the color specied by the argument.
	void clear(const QBrush& color);
	// Sets the cell specied by pos to the color specied by the color argument.
	void setPixel(const QVector2D& pos, const QBrush& color);
	// Returns the size of a single cell calculated from the height and
	// gridSize of the widget.
	int cellSize() const;
private:
	QSize gridSize;
	QVector<QVector<QGraphicsRectItem*>> grid;
};

#endif // PIXELVIEW_HH
