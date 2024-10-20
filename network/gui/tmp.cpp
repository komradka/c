void clear()
{
    coords.clear();
    objects_count[network_objects::sink] = 0;
    objects_count[network_objects::source] = 0;
}

void DrawLineWithArrow(QPainter &painter, QPoint start, QPoint end)
{

    painter.setRenderHint(QPainter::Antialiasing, true);

    double arrowSize = 10; // size of head
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    QLineF line(end, start);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << line.p1() << arrowP1 << arrowP2;
    painter.drawLine(line);
    painter.drawPolygon(arrowHead);
}

double calc_distance(QPoint x, QPoint y)
{
    return sqrt((y.x() - x.x()) * (y.x() - x.x()) + (y.y() - x.y()) * (y.y() - x.y()));
}

int find_index_of_min(std::vector<double> &v)
{
    int index = 0;
    double min = v[0];

    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] < min)
        {
            min = v[i];
            index = i;
        }
    }
    return index;
}

QPoint calculate_default_coords(network_objects type)
{
    return QPoint(default_coord.at(type).x(), default_coord.at(type).y() * objects_count.at(type));
}

void draw_object(QPainter &p, object_id obj)
{
    network_objects type = topology->get_type(obj);

    p.setBrush(QBrush(object_color[type], Qt::SolidPattern));

    QPoint coord = coords.find(obj) != coords.end() ? coords.at(obj) : calculate_default_coords(type);

    coords[obj] = coord;

    p.drawEllipse(coord, object_size.first, object_size.second);
}

std::pair<QPoint, QPoint> find_nearest_v(QPoint first, QPoint second)
{
    int fx = first.x();
    int fy = first.y();
    int sx = second.x();
    int sy = second.y();

    int size = object_size.first;

    std::vector<QPoint> v1 = {QPoint(fx + size, fy), QPoint(fx - size, fy), QPoint(fx, fy + size), QPoint(fx, fy - size)};

    std::vector<double> r(4);
    for (int i = 0; i < 4; i++)
    {
        r[i] = calc_distance(v1[i], second);
    }

    int min_index_1 = find_index_of_min(r);

    std::vector<QPoint> v2 = {QPoint(sx + size, sy), QPoint(sx - size, sy), QPoint(sx, sy + size), QPoint(sx, sy - size)};

    for (int i = 0; i < 4; i++)
    {
        r[i] = calc_distance(v2[i], first);
    }

    int min_index_2 = find_index_of_min(r);

    return make_pair(v1[min_index_1], v2[min_index_2]);
}

void draw_link(QPainter &p, link_id link)
{
    std::pair<object_id, object_id> objs = topology->get_connected_object(link);

    QPoint first = coords.at(objs.first);
    QPoint second = coords.at(objs.second);

    auto v = find_nearest_v(first, second);

    DrawLineWithArrow(p, v.first, v.second);
}

protected:
void paintEvent(QPaintEvent *event)
{
    if (topology == nullptr)
        return;

    clear();

    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    std::vector<object_id> objects = topology->get_objects();

    for (auto object : objects)
    {
        objects_count[topology->get_type(object)]++;
        draw_object(painter, object);
    }

    std::vector<link_id> links = topology->get_links();

    for (auto link : links)
    {
        draw_link(painter, link);
    }
}