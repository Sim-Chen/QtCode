#include "diagramwindow.h"
#include<QtWidgets>
#include"node.h"
#include"link.h"
diagramwindow::diagramwindow()
{
    scene =new QGraphicsScene(0,0,600,500);

    view=new QGraphicsView;

    view->setScene(scene);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
    view->setContextMenuPolicy(Qt::ActionsContextMenu);
    setCentralWidget(view);

    minZ=0;
    maxZ=0;
    seqNumber=0;

    createActions();
    createMenus();
    creatToolBar();

    connect(scene,SIGNAL(selectionChanged()),this,SLOT(updateActions()));

    setWindowTitle(tr("Diagram"));
    updateActions();

}


void  diagramwindow::createActions()
{
    exitAction=new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
    
    addNodeAction=new QAction(tr("add &Node"),this);
    addNodeAction->setIcon(QIcon(":/images/node.png"));
    addNodeAction->setShortcut(tr("Ctrl+N"));
    connect(addNodeAction,SIGNAL(triggered()),this,SLOT(addNode()));
    
    addLinkAction=new QAction(tr("add &Link"),this);
    addLinkAction->setIcon(QIcon(":/images/link.png"));
    connect(addLinkAction,SIGNAL(triggered()),this,SLOT(addLink()));
    
    deleteAction=new QAction(tr("Delete"),this);
    deleteAction->setIcon(QIcon(":/images/delete.png"));
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction,SIGNAL(triggered()),this,SLOT(del()));
    
    cutAction=new QAction(tr("Cu&t"),this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(tr("Ctrl+X"));
    connect(cutAction,SIGNAL(triggered()),this,SLOT(cut()));

    copyAction=new QAction(tr("&Copy"),this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(tr("Ctrl+C"));
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));

    pasteAction=new QAction(tr("&Paste"),this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(tr("Ctrl+V"));
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(paste()));

    bringToFrontAction=new QAction(tr("Bring to &Front"),this);
    bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
    connect(bringToFrontAction,SIGNAL(triggered()),this,SLOT(bringToFront()));


    sendToBackAction=new QAction(tr("Send to &Back"),this);
    sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
    connect(sendToBackAction,SIGNAL(triggered()),this,SLOT(sendToBack()));

    propertiesAction=new QAction(tr("Property"),this);
    connect(propertiesAction,SIGNAL(triggered()),this,SLOT(properties()));


}
/////////slots/////////////

void  diagramwindow::addNode()
{
    Node *node = new Node;
    node->setText(tr("Node %1").arg(seqNumber+1));
    setupNode(node);
}

void diagramwindow::addLink()
{
   NodePair  nodes=selectedNodePair();
   if(nodes==NodePair())
       return ;
    Link *link=new Link(nodes.first,nodes.second);
    scene->addItem(link);
   
}
    
void diagramwindow::del()
{
    QList<QGraphicsItem *>items=scene->selectedItems();
    QMutableListIterator <QGraphicsItem *>i(items);
    while(i.hasNext())
    {
        Link *link = dynamic_cast<Link *>(i.next());
        if(link){
            delete link;
            i.remove();
        }

    }
   qDeleteAll(items);
}

void diagramwindow::cut()
{
    Node *node=selectedNode();
    if(!node)
        return ;
    copy();
    delete node;
}

void diagramwindow::copy()
{
    Node *node=selectedNode();
    if(!node)
        return ;
    QString str=QString("Node %1 %2 %3 %4")
            .arg(node->textColor().name())
            .arg(node->outlineColor().name())
            .arg(node->backgroundColor().name())
            .arg(node->text());
    QApplication::clipboard()->setText(str);



}

void diagramwindow::paste()
{
    QString str=QApplication::clipboard()->text();
    QStringList parts=str.split(" ");
    if(parts.count()>=5&&parts.first()=="Node"){
        Node *node=new Node;
        node->setText(QStringList(parts.mid(4)).join(""));
        node->setTextColor(QColor(parts[1]));
        node->setOutlineColor(QColor(parts[2]));
        node->setBackgroundColor(QColor(parts[3]));
    }

}

void diagramwindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void diagramwindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void diagramwindow::properties()
{

}


void diagramwindow::updateActions()
{
    bool hasSelection=!scene->selectedItems().isEmpty();
    bool isNode=(selectedNode()!=0);
    bool isNodePair=(selectedNodePair()!=NodePair());

    cutAction->setEnabled(isNode);
    copyAction->setEnabled(isNode);
    addLinkAction->setEnabled(isNodePair);
    deleteAction->setEnabled(hasSelection);
    bringToFrontAction->setEnabled(isNode);
    sendToBackAction->setEnabled(isNode);
    propertiesAction->setEnabled(isNode);

    foreach (QAction *action, view->actions()) {
        view->removeAction(action);

    }
    foreach(QAction *action,editMenu->actions()){
        if(action->isEnabled())
            view->addAction(action);
    }


}
////////////////////////////

void diagramwindow::setupNode(Node *node)
{
    node->setPos(QPoint(80+100*(seqNumber%5),80+50*((seqNumber/5)%7)));
    scene->addItem(node);
    ++seqNumber;
    
    scene->clearSelection();
    node->setSelected(true);
    bringToFront();
    
}


void diagramwindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("File"));
    fileMenu->addAction(exitAction);
    editMenu=menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(addNodeAction);
    editMenu->addAction(addLinkAction);
    editMenu->addAction(deleteAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(bringToFrontAction);
    editMenu->addAction(sendToBackAction);
    editMenu->addSeparator();
    editMenu->addAction(propertiesAction);
}


void diagramwindow::creatToolBar()
{
    editToolBar = addToolBar(tr("Edit"));

    editToolBar->addAction(addNodeAction);
    editToolBar->addAction(addLinkAction);
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(bringToFrontAction);
    editToolBar->addAction(sendToBackAction);

}

Node *diagramwindow::selectedNode() const
{
    QList<QGraphicsItem*> items=scene->selectedItems();

   if(items.count()==1)
   {
       return dynamic_cast<Node *>(items.first());
   }
   else
       return 0;

}

Link *diagramwindow::selectedLink() const
{
    QList<QGraphicsItem*> items=scene->selectedItems();

   if(items.count()==1)
   {
       return dynamic_cast<Link *>(items.first());
   }
   else
       return 0;
}

diagramwindow::NodePair diagramwindow::selectedNodePair()const
{
    QList<QGraphicsItem *> items=scene->selectedItems();
    if(items.count()==2){
        Node *first=dynamic_cast<Node *>(items.first());
        Node *second=dynamic_cast<Node *>(items.last());
        if(first&&second)
            return NodePair(first,second);
    }

    return NodePair();


}


void diagramwindow::setZValue(int z)
{
    Node *node =selectedNode();
    if(node)
        node->setZValue(z);

}
