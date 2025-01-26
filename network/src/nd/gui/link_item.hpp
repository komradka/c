class link_item : public QGraphicsLineItem
{
public:
    bool is_clicked = false;

public:
    link_item(QLineF &line)
    {
        this->setLine(line);
    }

    void redrow(QLineF &line)
    {
        QPen clicked_drow = QPen(Qt::darkYellow, 3);
        QPen default_drow = QPen(Qt::black);
        
        setPen(is_clicked ? clicked_drow : default_drow);

        setLine(line);
    }

    ~link_item()
    {
    }

};
