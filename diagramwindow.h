#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include<QPair>
class QMenu;
class QToolBar;
class QAction;
class QGraphicsScene;
class Link;
class Node;
class QGraphicsView;
class diagramwindow : public QMainWindow
{
    Q_OBJECT
public:
   diagramwindow();



signals:

public slots:
    void addNode();
    void bringToFront();
    void addLink();
    void del();
    void cut();
    void copy();
    void paste();
    void sendToBack();
    void properties();
    void updateActions();

private:
    typedef QPair<Node *,Node *> NodePair;
    void setupNode(Node *node);
    Node *selectedNode() const;
    Link *selectedLink() const;
    NodePair selectedNodePair() const;
    void setZValue(int z);
    void createMenus();
    void createActions();
    void creatToolBar();

    QMenu *fileMenu;
    QMenu *editMenu;
    QToolBar *editToolBar;
    QAction *exitAction;
    QAction *addNodeAction;
    QAction *addLinkAction;
    QAction *deleteAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
    QAction *propertiesAction;

    int minZ;
    int maxZ;
    int seqNumber;
    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // DIAGRAMWINDOW_H
