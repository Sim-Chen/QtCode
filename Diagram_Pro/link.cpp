#include "link.h"
#include"node.h"
#include<QtWidgets>
Link::Link(Node *FromNode,Node *ToNode)
{
    myFromNode=FromNode;
    myToNode =ToNode;


    myFromNode->addLink(this);
    myToNode->addLink(this);

    setFlags(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);

    setColor(Qt::darkRed);
    trackNodes();

}
Link::~Link()
{
    myFromNode->removeLink(this);
    myToNode->removeLink(this);

}
void Link::setColor(const QColor &color)
{
     setPen(QPen(color,1.0));
}

Node *Link::fromNode() const
{
    return myFromNode;

}
Node *Link ::toNode() const
{
    return myToNode;
}

QColor Link::color() const
{
    return pen().color();
}

void Link::trackNodes()
{
    setLine(QLineF(myFromNode->pos(),myToNode->pos()));

}
