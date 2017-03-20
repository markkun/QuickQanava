/*
    This file is part of QuickQanava library.

    Copyright (C) 2008-2017 Benoit AUTHEMAN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//-----------------------------------------------------------------------------
// This file is a part of the QuickQanava software library.
//
// \file	qanGraph.cpp
// \author	benoit@destrat.io
// \date	2004 February 15
//-----------------------------------------------------------------------------

// Qt headers
#include <QQmlProperty>
#include <QVariant>
#include <QQmlEngine>
#include <QQmlComponent>

<<<<<<< HEAD
// GTpo headers
#include "gtpoRandomGraph.h"

// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNavigable.h"
=======
// QuickQanava headers
#include "./qanGraph.h"
#include "./qanNavigable.h"
#include "./qanNodeItem.h"
#include "./qanEdgeItem.h"
#include "./qanGroup.h"
#include "./qanGroupItem.h"
#include "./qanConnector.h"
>>>>>>> dev

namespace qan { // ::qan

/* Graph Object Management *///------------------------------------------------
Graph::Graph( QQuickItem* parent ) noexcept :
<<<<<<< HEAD
    gtpo::GenGraph< qan::Config >( parent ),
    _styleManager{ SharedStyleManager{ new qan::StyleManager( this ) } }
=======
    gtpo::GenGraph< qan::GraphConfig >( parent )
>>>>>>> dev
{
    setContainerItem( this );
    setAntialiasing( true );
    setSmooth( true );
}

<<<<<<< HEAD
void    Graph::qmlClearGraph( ) noexcept
{
    qan::Graph::clear();
}

void    Graph::clear( ) noexcept
{
    _selectedNodes.clear();
    gtpo::GenGraph< qan::Config >::clear();
    _styleManager->clear();
}

void    Graph::addControlNode( qan::Node* node )
{
    if ( node != nullptr )
        gtpo::GenGraph< qan::Config >::addControlNode( node->shared_from_this( ) );
}

void    Graph::removeControlNode( qan::Node* node )
{
    if ( node == nullptr )
        return;
    gtpo::GenGraph< qan::Config >::removeControlNode( node->shared_from_this( ) );
=======
Graph::~Graph() { /* Nil */ }

void    Graph::componentComplete()
{
    const auto engine = qmlEngine(this);
    if ( engine ) {
        setNodeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Node.qml")));
        setEdgeDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Edge.qml")));
        setGroupDelegate(std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/Group.qml")));

        _styleManager.setStyleComponent(qan::Node::style(), qan::Node::delegate(this) );
        _styleManager.setStyleComponent(qan::Edge::style(), qan::Edge::delegate(this) );

        // Visual connector initialization
        auto connectorComponent = std::make_unique<QQmlComponent>(engine, QStringLiteral("qrc:/QuickQanava/VisualConnector.qml"));
        if ( connectorComponent ) {
            qan::Style* style = qan::Connector::style();
            if ( style != nullptr ) {
                _connector.reset( qobject_cast<qan::Connector*>(createFromComponent(connectorComponent.get(), *style, nullptr)) );
                emit connectorChanged();
                if (_connector)
                    _connector->setGraph(this);
                _connector->setEnabled(getConnectorEnabled());
                _connector->setVisible(getConnectorEnabled());
            } else qWarning() << "qan::Graph::componentComplete(): Error: No style available for connector creation.";
        }
    } else qWarning() << "qan::Graph::componentComplete(): Error: No QML engine available to register default QML delegates.";
}

void    Graph::qmlClearGraph() noexcept
{
    qan::Graph::clear();
}

void    Graph::clear() noexcept
{
    _selectedNodes.clear();
    gtpo::GenGraph< qan::GraphConfig >::clear();
    _styleManager.clear();
>>>>>>> dev
}

QQuickItem* Graph::graphChildAt(qreal x, qreal y) const
{
    if ( getContainerItem() == nullptr )
        return nullptr;

    const QList<QQuickItem *> children = getContainerItem()->childItems();
    //qDebug() << "graphChildAt(): x=" << x << " y=" << y;
    //qDebug() << " \tchildren=" << children;
    for (int i = children.count()-1; i >= 0; --i) {
        QQuickItem *child = children.at(i);
        QPointF point = mapToItem( child, QPointF(x, y) );  // Map coordinates to the child element's coordinate space
        if ( child->isVisible() &&
             child->contains( point ) &&    // Note 20160508: childAt do not call contains()
             point.x() >= 0 &&
             child->width() > point.x() &&
             point.y() >= 0 &&
             child->height() > point.y() ) {
            if ( child->inherits( "qan::Group" ) ) {  // For group, look in group childs
                qan::Group* group = qobject_cast<qan::Group*>( child );
<<<<<<< HEAD
                if ( group != nullptr && group->getContainer() != nullptr ) {
=======
                // FIXME QAN3
                /*
                if ( group != nullptr &&
                     group->getContainer() != nullptr ) {
>>>>>>> dev
                    const QList<QQuickItem *> groupChildren = group->getContainer()->childItems();
                    for (int gc = groupChildren.count()-1; gc >= 0; --gc) {
                        QQuickItem *groupChild = groupChildren.at(gc);
                        point = mapToItem( groupChild, QPointF(x, y) ); // Map coordinates to group child element's coordinate space
                        if ( groupChild->isVisible() &&
                             groupChild->contains( point ) &&
                             point.x() >= 0 &&
                             groupChild->width() > point.x() &&
                             point.y() >= 0 &&
                             groupChild->height() > point.y() ) {
                            return groupChild;
                        }
                    }
<<<<<<< HEAD
                }
=======
                }*/
>>>>>>> dev
            }
            return child;
        }
    }
    return nullptr;
}

qan::Group* Graph::groupAt( const QPointF& p, const QSizeF& s ) const
{
<<<<<<< HEAD
    for ( const auto& g : getGroups() ) {
        if ( g == nullptr )
            continue;
        if ( QRectF{ g->position(), QSizeF{ g->width(), g->height() } }.contains( QRectF{ p, s } ) )
            return g.get();
    }
=======
    for ( const auto& group : getGroups() )
        if ( group &&
             group->getItem() != nullptr ) {
            const auto groupItem = group->getItem();
             if ( QRectF{ groupItem->position(),
                  QSizeF{ groupItem->width(), groupItem->height() } }.contains( QRectF{ p, s } ) )
                 return group.get();
        }
>>>>>>> dev
    return nullptr;
}

void    Graph::setContainerItem( QQuickItem* containerItem )
{
    if ( containerItem != nullptr &&
         containerItem != _containerItem.data() ) {
        _containerItem = containerItem;
        emit containerItemChanged();
    }
}
//-----------------------------------------------------------------------------

<<<<<<< HEAD
/* Selection Management *///---------------------------------------------------
void    Graph::setSelectionPolicy( SelectionPolicy selectionPolicy )
{
    if ( selectionPolicy == _selectionPolicy )  // Binding loop protection
        return;
    _selectionPolicy = selectionPolicy;
    if ( selectionPolicy == SelectionPolicy::NoSelection )
        clearSelection();
    emit selectionPolicyChanged( );
}

void    Graph::setSelectionColor( QColor selectionColor )
{
    _selectionColor = selectionColor;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getSelectionItem() != nullptr ) {
            QQuickItem* selectionItem = node->getSelectionItem();
            selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
            QObject* rectangleBorder = node->getSelectionItem()->property( "border" ).value<QObject*>();
            if ( rectangleBorder != nullptr )
                rectangleBorder->setProperty( "color", selectionColor );
        }
    }
    emit selectionColorChanged();
}

void    Graph::setSelectionWeight( qreal selectionWeight )
{
    if ( qFuzzyCompare( selectionWeight, _selectionWeight ) )   // Never 0
        return;
    _selectionWeight = selectionWeight;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr )
            node->configureSelectionItem( selectionWeight, getSelectionMargin() );
    }
    emit selectionWeightChanged();
}

void    Graph::setSelectionMargin( qreal selectionMargin )
{
    if ( qFuzzyCompare( selectionMargin, _selectionMargin ) )   // Never 0
        return;
    _selectionMargin = selectionMargin;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr )
            node->configureSelectionItem( getSelectionWeight(), selectionMargin );
    }
    emit selectionMarginChanged();
}

bool    Graph::selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers )
{
    if ( getSelectionPolicy() == SelectionPolicy::NoSelection )
        return false;

    bool selectNode{ false };
    bool ctrlPressed = modifiers & Qt::ControlModifier;

    if ( node.getSelected() ) {
        if ( ctrlPressed )          // Click on a selected node + CTRL = deselect node
            removeFromSelection( node );
    } else {
        switch ( getSelectionPolicy() ) {
        case SelectionPolicy::SelectOnClick:
            selectNode = true;        // Click on an unselected node with SelectOnClick = select node
            if ( !ctrlPressed )
                clearSelection();
            break;
        case SelectionPolicy::SelectOnCtrlClick:
            selectNode = ctrlPressed; // Click on an unselected node with CTRL pressed and SelectOnCtrlClick = select node
            break;
        default: break;
        }
    }
    if ( selectNode ) {
        addToSelection( node );
        return true;
    }
    return false;
}

void    Graph::addToSelection( qan::Node& node )
{
    if ( !_selectedNodes.contains( &node ) ) {
        _selectedNodes.append( &node );
        node.configureSelectionItem( getSelectionColor(), getSelectionWeight(), getSelectionMargin() );
        node.setSelected( true );
    }
}

void    Graph::removeFromSelection( qan::Node& node )
{
    if ( _selectedNodes.contains( &node ) )
        _selectedNodes.remove( &node );
    node.setSelected( false );
}

void    Graph::clearSelection()
{
    for ( auto& node : _selectedNodes )
        if ( node != nullptr )
            node->setSelected( false );
    _selectedNodes.clear();
}

void    Graph::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton ) {
        clearSelection();
        forceActiveFocus();
    }
    event->ignore();
    qan::Config::GraphBase::mousePressEvent( event );
}
//-----------------------------------------------------------------------------

/* Delegates Management *///---------------------------------------------------
auto    Graph::createFromDelegate( QQmlComponent* component ) -> QQuickItem*
{
    if ( component == nullptr ) {
        qDebug() << "qan::Graph::createFromDelegate(): Error called with a nullptr delegate component";
        return nullptr;
    }
    QQuickItem* item = nullptr;
    //auto* context = QQmlEngine::contextForObject(this);
    if ( component->isReady() ) {
        //QObject* object = component->create(context);
        QObject* object = component->create();
        if ( object != nullptr && !component->isError() ) {
            QQmlEngine::setObjectOwnership( object, QQmlEngine::CppOwnership );
            item = qobject_cast< QQuickItem* >( object );
            item->setVisible( true );
            item->setParentItem( getContainerItem() );
        } else {
            qDebug() << "qan::Graph::createFromDelegate(): Failed to create a concrete QQuickItem from QML component:";
            qDebug() << "\t" << component->errorString();
        }
    } else {
        qDebug() << "qan::Graph::createFromDelegate(): QML component is not ready:";
        qDebug() << "\t" << component->errorString();
=======

/* Visual connection Management *///-------------------------------------------
void    Graph::setConnectorSource(qan::Node* sourceNode) noexcept
{
    if ( _connector ) {
        if ( sourceNode != nullptr )
            _connector->setSourceNode(sourceNode);
        _connector->setVisible(getConnectorEnabled());
        _connector->setEnabled(getConnectorEnabled());
    }
}

void    Graph::setConnectorEnabled( bool connectorEnabled ) noexcept
{
    if ( connectorEnabled != _connectorEnabled ) {
        _connectorEnabled = connectorEnabled;
        if ( _connector ) {
            _connector->setVisible(connectorEnabled);
            _connector->setEnabled(connectorEnabled);
        }
        emit connectorEnabledChanged();
    }
}
qan::Connector* Graph::getConnector() noexcept  {  return _connector.data(); }
//-----------------------------------------------------------------------------

/* Delegates Management *///---------------------------------------------------
void    Graph::setNodeDelegate(QQmlComponent* nodeDelegate) noexcept
{
    if ( nodeDelegate != nullptr ) {
        if ( nodeDelegate != _nodeDelegate.get() ) {
            _nodeDelegate.reset(nodeDelegate);
            QQmlEngine::setObjectOwnership( nodeDelegate, QQmlEngine::CppOwnership );
            emit nodeDelegateChanged();
        }
    }
}

void    Graph::setNodeDelegate(std::unique_ptr<QQmlComponent> nodeDelegate) noexcept
{
    setNodeDelegate(nodeDelegate.release());
}

void    Graph::setEdgeDelegate(QQmlComponent* edgeDelegate) noexcept
{
    if ( edgeDelegate != nullptr ) {
        if ( edgeDelegate != _edgeDelegate.get() ) {
            _edgeDelegate.reset(edgeDelegate);
            QQmlEngine::setObjectOwnership( edgeDelegate, QQmlEngine::CppOwnership );
            emit edgeDelegateChanged();
        }
    }
}

void    Graph::setEdgeDelegate(std::unique_ptr<QQmlComponent> edgeDelegate) noexcept
{
    setEdgeDelegate(edgeDelegate.release());
}

void    Graph::setGroupDelegate(QQmlComponent* groupDelegate) noexcept
{
    if ( groupDelegate != nullptr ) {
        if ( groupDelegate != _groupDelegate.get() ) {
            _groupDelegate.reset(groupDelegate);
            QQmlEngine::setObjectOwnership( groupDelegate, QQmlEngine::CppOwnership );
            emit groupDelegateChanged();
        }
    }
}

void    Graph::setGroupDelegate(std::unique_ptr<QQmlComponent> groupDelegate) noexcept
{
    setGroupDelegate(groupDelegate.release());
}

QQuickItem* Graph::createFromComponent( QQmlComponent* component,
                                        qan::Style& style,
                                        qan::Node* node,
                                        qan::Edge* edge,
                                        qan::Group* group )
{
    if ( component == nullptr ) {
        qWarning() << "qan::Graph::createFromComponent(): Error called with a nullptr delegate component.";
        return nullptr;
    }
    QQuickItem* item = nullptr;
    if ( component->isReady() ) {
        const auto rootContext = qmlContext(this);
        if ( rootContext == nullptr ) {
            qWarning() << "qan::Graph::createFromComponent(): Error can't access to local QML context.";
            return nullptr;
        }
        QObject* object = component->beginCreate(rootContext);
        if ( object != nullptr &&
             !component->isError() ) {
            if ( node != nullptr ) {
                const auto nodeItem = qobject_cast<qan::NodeItem*>(object);
                if ( nodeItem != nullptr ) {
                    nodeItem->setNode(node);
                    nodeItem->setGraph(this);
                    _styleManager.setStyleComponent(&style, component );
                    nodeItem->setStyle(qobject_cast<qan::NodeStyle*>(&style));
                }
            } else if ( edge != nullptr ) {
                const auto edgeItem = qobject_cast<qan::EdgeItem*>(object);
                if ( edgeItem != nullptr ) {
                    edgeItem->setEdge(edge);
                    edgeItem->setGraph(this);
                    _styleManager.setStyleComponent(edgeItem->getStyle(), component );
                }
            } else if ( group != nullptr ) {
                const auto groupItem = qobject_cast<qan::GroupItem*>(object);
                if ( groupItem != nullptr ) {
                    groupItem->setGroup(group);
                    groupItem->setGraph(this);
                    _styleManager.setStyleComponent(groupItem->getStyle(), component );
                }
            }
            component->completeCreate();
            if ( !component->isError() ) {
                QQmlEngine::setObjectOwnership( object, QQmlEngine::CppOwnership );
                item = qobject_cast< QQuickItem* >( object );
                item->setVisible( true );
                item->setParentItem( getContainerItem() );
            } // Note QAN3: There is no leak until cpp ownership is set
        } else {
            qWarning() << "qan::Graph::createFromComponent(): Failed to create a concrete QQuickItem from QML component:";
            qWarning() << "\t" << component->errorString();
        }
    } else {
        qWarning() << "qan::Graph::createFromComponent(): QML component is not ready:";
        qWarning() << "\t" << component->errorString();
>>>>>>> dev
    }

    return item;
}

<<<<<<< HEAD
void Graph::setCppOwnership( QQuickItem* item )
{
    if ( item == nullptr )
        return;
    QQmlEngine::setObjectOwnership( item, QQmlEngine::CppOwnership );
    for ( auto childItem: item->childItems() )
        qan::Graph::setCppOwnership( childItem );
}

void    Graph::registerNodeDelegate( QString nodeClass, QQmlComponent* nodeComponent )
{
    if ( nodeClass.isEmpty() || nodeComponent == nullptr ) {
        qDebug() << "qan::Graph::registerNodeDelegate(): Error: Trying to register a node delegate with empty class name or null delegate component";
        return;
    }
    _nodeClassComponents.insert( nodeClass, nodeComponent );
}

void    Graph::registerEdgeDelegate( QString edgeClass, QQmlComponent* edgeComponent )
{
    if ( edgeClass.isEmpty() || edgeComponent == nullptr ) {
        qDebug() << "qan::Graph::registerEdgeDelegate(): Error: Trying to register an edge delegate with empty class name or null delegate component";
        return;
    }
    _edgeClassComponents.insert( edgeClass, edgeComponent );
}

void    Graph::registerGroupDelegate( QString groupClass, QQmlComponent* groupComponent )
{
    if ( groupClass.isEmpty() || groupComponent == nullptr ) {
        qDebug() << "qan::Graph::registerGroupDelegate(): Error: Trying to register a group delegate with empty class name or null delegate component";
        return;
    }
    _groupClassComponents.insert( groupClass, groupComponent );
}

QQuickItem* Graph::createNodeItem( QString nodeClass )
{
    if ( _nodeClassComponents.contains( nodeClass ) )
        return createFromDelegate( _nodeClassComponents.value( nodeClass ) );
    return new QQuickItem();    // Is should never happen, otherwise it is leaked.
}

QQuickItem* Graph::createEdgeItem( QString edgeClass )
{
    if ( _edgeClassComponents.contains( edgeClass ) )
        return createFromDelegate( _edgeClassComponents.value( edgeClass ) );
    return new QQuickItem();    // Is should never happen, otherwise it is leaked.
=======
QQuickItem* Graph::createFromComponent( QQmlComponent* component, qan::Style* style )
{
    return ( component != nullptr &&
             style != nullptr ) ? createFromComponent( component, *style, nullptr, nullptr, nullptr ) : nullptr;
}


void    Graph::setCppOwnership( QQuickItem* item )
{
    if ( item == nullptr )
        return;
    QQmlEngine::setObjectOwnership( item, QQmlEngine::CppOwnership );
    for ( auto childItem: item->childItems() )
        qan::Graph::setCppOwnership( childItem );
>>>>>>> dev
}

QQuickItem* Graph::createRectangle( QQuickItem* parent )
{
    // Initialize rectangle component for the first factory call.
    if ( _rectangleComponent == nullptr ) {
        QQmlEngine* engine = qmlEngine( this );
        if ( engine != nullptr ) {
            _rectangleComponent = std::make_unique<QQmlComponent>(engine);
<<<<<<< HEAD
            QString componentQml = "import QtQuick 2.6\n  Rectangle{ }";
            _rectangleComponent ->setData( componentQml.toUtf8(), QUrl() );
            if ( !_rectangleComponent->isReady() ) {
                qDebug() << "qan::Graph::createRectangle(): Error: Can't create at Qt Quick Rectangle object:";
                qDebug() << componentQml;
                qDebug() << "QML Component status=" << _rectangleComponent->status();
                qDebug() << "QML Component errors=" << _rectangleComponent->errors();
=======
            QString componentQml = "import QtQuick 2.7\n  Rectangle{ }";
            _rectangleComponent->setData( componentQml.toUtf8(), QUrl() );
            if ( !_rectangleComponent->isReady() ) {
                qWarning() << "qan::Graph::createRectangle(): Error: Can't create at Qt Quick Rectangle object:";
                qWarning() << "\t" << componentQml;
                qWarning() << "\tQML Component status=" << _rectangleComponent->status();
                qWarning() << "\tQML Component errors=" << _rectangleComponent->errors();
>>>>>>> dev
            }
        }
    }

    // Create a Qt Quick Rectangle object
    if ( _rectangleComponent != nullptr &&
         _rectangleComponent->isReady() ) {
        QObject* object = _rectangleComponent->create();
        QQuickItem* rectangle = qobject_cast< QQuickItem* >( object );
        if ( rectangle != nullptr ) {
            rectangle->setVisible( false );
<<<<<<< HEAD
=======
            rectangle->setEnabled( false ); // Avoid node/edge/group selection problems
>>>>>>> dev
            if ( parent != nullptr )
                rectangle->setParentItem( parent );
            QQmlEngine::setObjectOwnership( rectangle, QQmlEngine::CppOwnership );
        }
        if ( rectangle == nullptr && object != nullptr ) {
<<<<<<< HEAD
            qDebug() << "qan::Graph::createRectangle(): Error: A Qt Quick Object has been created, but it is not a Quick Canvas.";
=======
            qWarning() << "qan::Graph::createRectangle(): Error: A Qt Quick Object has been created, but it is not a Quick Canvas.";
>>>>>>> dev
            delete object;      // Somtehing has been created, but it is not a rectangle !
            return nullptr;
        }
        return rectangle;
    } else {
<<<<<<< HEAD
        qDebug() << "qan::Graph::createRectangle(): Error: Can't create a Qt Quick Rectangle Object.";
=======
        qWarning() << "qan::Graph::createRectangle(): Error: Can't create a Qt Quick Rectangle Object.";
>>>>>>> dev
    }
    return nullptr;
}
//-----------------------------------------------------------------------------

/* Graph Factories *///--------------------------------------------------------
qan::Node*  Graph::insertNode( QQmlComponent* nodeComponent )
{
<<<<<<< HEAD
    if ( nodeComponent == nullptr ) {
        if ( _nodeClassComponents.contains( "qan::Node" ) )
            nodeComponent = _nodeClassComponents.value( "qan::Node", nullptr );
    }
    if ( nodeComponent == nullptr ) {
        qDebug() << "qan::Graph::insertNode(): Error: Can't find a valid node delegate component.";
        return nullptr;
    }
    qan::Node* node = static_cast< qan::Node* >( createFromDelegate( nodeComponent ) );
    node->setSelectionItem( createRectangle( node ) );
    if ( node != nullptr ) {
        GTpoGraph::insertNode( std::shared_ptr<qan::Node>{node} );

        connect( node, &qan::Node::nodeClicked, this, &qan::Graph::nodeClicked );
        connect( node, &qan::Node::nodeRightClicked, this, &qan::Graph::nodeRightClicked );
        connect( node, &qan::Node::nodeDoubleClicked, this, &qan::Graph::nodeDoubleClicked );
    } else
        qDebug() << "qan::Graph::insertNode(): Warning: Node creation failed with the corresponding delegate";
    return node;
}

qan::Graph::WeakNode    Graph::insertNode( SharedNode node )
{
    return GTpoGraph::insertNode( node );
}

qan::Node*  Graph::insertNode( QString nodeClassName )
{
    if ( nodeClassName.isEmpty() )
        return nullptr;
    QQmlComponent* nodeComponent = nullptr;
    if ( _nodeClassComponents.contains( nodeClassName ) )
        nodeComponent = _nodeClassComponents.value( nodeClassName, nullptr );
    if ( nodeComponent == nullptr )
        return nullptr;
    qan::Node* node = static_cast< qan::Node* >( createFromDelegate( nodeComponent ) );
    node->setSelectionItem( createRectangle( node ) );
    SharedNode sharedNode{ node };
    if ( node != nullptr ) {
        GTpoGraph::insertNode( sharedNode );
        qan::NodeStyle* defaultStyle = qobject_cast< qan::NodeStyle* >( getStyleManager()->getDefaultNodeStyle( nodeClassName ) );
        if ( defaultStyle != nullptr )
            node->setStyle( defaultStyle );
        connect( node, &qan::Node::nodeClicked, this, &qan::Graph::nodeClicked );
        connect( node, &qan::Node::nodeRightClicked, this, &qan::Graph::nodeRightClicked );
        connect( node, &qan::Node::nodeDoubleClicked, this, &qan::Graph::nodeDoubleClicked );
    }
    return node;
}

qan::Node*  Graph::insertNode( QVariant nodeArgument )
{
    if ( !nodeArgument.isValid() ) {
        qDebug() << "qan::Graph::insertNode(QVariant): Error: Called with invalid QVariant.";
        return nullptr;
    }
    if ( nodeArgument.type() == QVariant::String ) {
        return insertNode( nodeArgument.toString( ) );
    } else if ( nodeArgument.canConvert<QObject*>() ) {
        QQmlComponent* nodeComponent = qobject_cast< QQmlComponent* >( nodeArgument.value< QObject* >() );
        if ( nodeComponent != nullptr )
            return insertNode( nodeComponent );
    }
    qDebug() << "qan::Graph::insertNode(QVariant): Error: Called with unsupported QVariant argument type (must be either QString or QQmlComponent.";
    return nullptr;
}

Graph::WeakNode    Graph::createNode( const std::string& className )
{
    if ( className.size() == 0 )
        return WeakNode{};
    qan::Node* node = insertNode( QString::fromStdString( className ) );
    return ( node != nullptr ? node->shared_from_this() : WeakNode{} ); // Note 20160213: shared_from_this() could be used since the node has already been added to graph, it
=======
    return insertNode<qan::Node>(nodeComponent);
>>>>>>> dev
}

void    Graph::removeNode( qan::Node* node )
{
    if ( node == nullptr )
        return;
    WeakNode weakNode;
    try {
        weakNode = WeakNode{ node->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
<<<<<<< HEAD
    GTpoGraph::removeNode( weakNode );
}

bool    Graph::isNode( QQuickItem* item ) const
{
    if ( !item->inherits( "qan::Node" ) )
        return false;   // "Fast" exit if item is not a qan::Node...
    qan::Node* node = qobject_cast< qan::Node* >( item );
    if ( node == nullptr )
        return false;
    return GTpoGraph::contains( node->shared_from_this() );
}

bool    Graph::isEdge( QQuickItem* item ) const
{
    if ( !item->inherits( "qan::Edge" ) )
        return false;   // "Fast" exit if item is not a qan::Edge...
    qan::Edge* edge = qobject_cast< qan::Edge* >( item );
    if ( edge == nullptr )
        return false;
    return GTpoGraph::contains( edge->shared_from_this() );
}

bool    Graph::isHyperEdge( QQuickItem* item ) const
{
    if ( !isEdge(item)) // "Fast" exit if item is not a qan::Edge...
        return false;
    qan::Edge* edge = qobject_cast< qan::Edge* >( item );
    if ( edge == nullptr )
        return false;
    return ( edge == nullptr ||
             ( !edge->getHDst().expired() &&
               edge->getHDst().lock() != nullptr ) );
}
=======
    if ( _selectedNodes.contains(node) )
        _selectedNodes.remove(node);
    GTpoGraph::removeNode( weakNode );
}
>>>>>>> dev
//-----------------------------------------------------------------------------

/* Graph Edge Management *///--------------------------------------------------
qan::Edge*  Graph::insertEdge( QObject* source, QObject* destination, QQmlComponent* edgeComponent )
{
    auto sourceNode = qobject_cast<qan::Node*>(source);
    if ( sourceNode != nullptr ) {
            if ( qobject_cast<qan::Node*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Node*>( destination ), edgeComponent );
<<<<<<< HEAD
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    qDebug() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
=======
            else if ( qobject_cast<qan::Group*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Group*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                return insertEdge( sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    qWarning() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
>>>>>>> dev
    return nullptr;
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent )
{
<<<<<<< HEAD
    Q_UNUSED( edgeComponent );
    return insertEdge( "qan::Edge", source, destination );
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
{
    Q_UNUSED( edgeComponent );
    return insertEdge( "qan::Edge", source, destination );
}

qan::Edge*  Graph::insertEdge( QString edgeClassName, QObject* source, QObject* destination, QQmlComponent* edgeComponent )
{
    auto sourceNode = qobject_cast<qan::Node*>(source);
    if ( sourceNode != nullptr ) {
            if ( qobject_cast<qan::Node*>(destination) != nullptr )
                return insertEdge( edgeClassName, sourceNode, qobject_cast<qan::Node*>( destination ), edgeComponent );
            else if ( qobject_cast<qan::Edge*>(destination) != nullptr )
                return insertEdge( edgeClassName, sourceNode, qobject_cast<qan::Edge*>( destination ), edgeComponent );
    }
    qDebug() << "qan::Graph::insertEdge(): Error: Unable to find a valid insertEdge() method for arguments " << source << " and " << destination;
    return nullptr;
}

qan::Edge*  Graph::insertEdge( QString edgeClassName, qan::Node* source, qan::Node* destination, QQmlComponent* edgeComponent )
{
    if ( source == nullptr || destination == nullptr )
        return nullptr;
    if ( edgeComponent == nullptr && _edgeClassComponents.contains( edgeClassName ) )
        edgeComponent = _edgeClassComponents.value( edgeClassName, nullptr );
    if ( edgeComponent == nullptr )
        return nullptr;
    qan::Edge* edge = nullptr;
    try {
        edge = static_cast< qan::Edge* >( createFromDelegate( edgeComponent ) );
        if ( edge != nullptr ) {
            edge->setSourceItem( source );
            edge->setDestinationItem( destination );

            WeakNode sharedSource{ source->shared_from_this() };
            WeakNode sharedDestination{ destination->shared_from_this() };
            edge->setSrc( sharedSource );
            edge->setDst( sharedDestination );
            GTpoGraph::insertEdge( SharedEdge{edge} );  // Note: Do not use shared_from_this()

            qan::EdgeStyle* defaultStyle = qobject_cast< qan::EdgeStyle* >( getStyleManager()->getDefaultEdgeStyle( "qan::Edge" ) );
            if ( defaultStyle != nullptr )
                edge->setStyle( defaultStyle );

            edge->setVisible( true );
            edge->updateItem();

            connect( edge, SIGNAL( edgeClicked( QVariant, QVariant ) ), this, SIGNAL( edgeClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeRightClicked( QVariant, QVariant ) ), this, SIGNAL( edgeRightClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ), this, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ) );
        }
    } catch ( gtpo::bad_topology_error e ) {
        qDebug() << "qan::Graph::insertEdge(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qDebug() << "qan::Graph::insertEdge(): Error: Topology error.";
    }
    return edge;
}

qan::Edge*  Graph::insertEdge( QString edgeClassName, qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
=======
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
    return insertEdge<qan::Edge>(*source, destination, nullptr, edgeComponent );
}

qan::Edge*  Graph::insertEdge( qan::Node* source, qan::Edge* destination, QQmlComponent* edgeComponent )
>>>>>>> dev
{
    if ( source == nullptr ||
         destination == nullptr )
        return nullptr;
<<<<<<< HEAD
    if ( edgeComponent == nullptr &&
         _edgeClassComponents.contains( edgeClassName ) )
        edgeComponent = _edgeClassComponents.value( edgeClassName, nullptr );
    if ( edgeComponent == nullptr )
        return nullptr;
    qan::Edge* edge = nullptr;
    try {
        edge = static_cast< qan::Edge* >( createFromDelegate( edgeComponent ) );
        if ( edge != nullptr ) {
            edge->setSourceItem( source );
            edge->setDestinationEdge( destination );

            WeakNode sourcePtr{ source->shared_from_this() };
            WeakEdge destinationPtr{ destination->shared_from_this() };
            edge->setSrc( sourcePtr );
            edge->setHDst( destinationPtr );
            GTpoGraph::insertEdge( SharedEdge{edge} );  // Note: Do not use shared_from_this()

            qan::EdgeStyle* defaultStyle = qobject_cast< qan::EdgeStyle* >( getStyleManager()->getDefaultEdgeStyle( "qan::Edge" ) );
            if ( defaultStyle != nullptr )
                edge->setStyle( defaultStyle );
            edge->setVisible( true );
            edge->updateItem();
            connect( edge, SIGNAL( edgeClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeRightClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeRightClicked( QVariant, QVariant ) ) );
            connect( edge, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ),
                     this, SIGNAL( edgeDoubleClicked( QVariant, QVariant ) ) );
        }
    } catch ( gtpo::bad_topology_error e ) {
        qDebug() << "qan::Graph::insertEdge(): Error: Topology error:" << e.what();
    }
    catch ( ... ) {
        qDebug() << "qan::Graph::insertEdge(): Error: Topology error.";
    }
    return edge;
}

Graph::WeakEdge    Graph::createEdge( const std::string& className, WeakNode source, WeakNode destination )
{
    if ( className.size() == 0 )
        return WeakEdge();
    qan::Node* src = static_cast< qan::Node* >( source.lock().get() );
    qan::Node* dst = static_cast< qan::Node* >( destination.lock().get() );
    qan::Edge* edge = nullptr;
    if ( src != nullptr &&
         dst != nullptr )
        edge = insertEdge( QString::fromStdString( className ), src, dst );
    return ( edge != nullptr ? edge->shared_from_this() : WeakEdge() ); // Note 20160213: shared_from_this() could be used since the edge has already been added to graph, it
}

Graph::WeakEdge    Graph::createEdge( const std::string& className, WeakNode source, WeakEdge destination )
{
    if ( className.size() == 0 )
        return WeakEdge();
    auto src = static_cast< qan::Node* >( source.lock().get() );
    auto dst = static_cast< qan::Edge* >( destination.lock().get() );
    qan::Edge* edge = nullptr;
    if ( src != nullptr &&
         dst != nullptr )
        edge = insertEdge( QString::fromStdString( className ), src, dst );
    return ( edge != nullptr ? edge->shared_from_this() : WeakEdge() );
=======
    return insertEdge<qan::Edge>(*source, nullptr, destination, edgeComponent );
>>>>>>> dev
}

void    Graph::removeEdge( qan::Node* source, qan::Node* destination )
{
    if ( source == nullptr || destination == nullptr )
        return;
    WeakNode sharedSource;
    WeakNode sharedDestination;
    try {
        sharedSource = WeakNode{ source->shared_from_this() };
        sharedDestination = WeakNode{ destination->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return; }
    return GTpoGraph::removeEdge( sharedSource, sharedDestination );
}

void    Graph::removeEdge( qan::Edge* edge )
{
<<<<<<< HEAD
    if ( edge != nullptr )
        GTpoGraph::removeEdge( edge->shared_from_this() );
=======
    using WeakEdge = std::weak_ptr<qan::Edge>;
    if ( edge != nullptr )
        GTpoGraph::removeEdge( WeakEdge{edge->shared_from_this()} );
>>>>>>> dev
}

bool    Graph::hasEdge( qan::Node* source, qan::Node* destination ) const
{
    if ( source == nullptr || destination == nullptr )
        return false;
    WeakNode sharedSource;
    WeakNode sharedDestination;
    try {
        sharedSource = WeakNode{ source->shared_from_this() };
        sharedDestination = WeakNode{ destination->shared_from_this() };
    } catch ( std::bad_weak_ptr e ) { return false; }
    return GTpoGraph::hasEdge( sharedSource, sharedDestination );
}
//-----------------------------------------------------------------------------

/* Graph Group Management *///-------------------------------------------------
<<<<<<< HEAD
Graph::WeakGroup   Graph::createGroup( const std::string& className )
{
    if ( className.size() == 0 )
        return WeakGroup{};
    qan::Group* group = insertGroup( QString::fromStdString( className ) );
    if ( group != nullptr ) {
        connect( group, &qan::Group::groupClicked, this, &qan::Graph::groupClicked );
        connect( group, &qan::Group::groupRightClicked, this, &qan::Graph::groupRightClicked );
        connect( group, &qan::Group::groupDoubleClicked, this, &qan::Graph::groupDoubleClicked );
    }
    return ( group != nullptr ? group->shared_from_this() : WeakGroup{} );
}

qan::Group* Graph::insertGroup( QString className )
{
    QQmlComponent* groupComponent{ nullptr };
    groupComponent = _groupClassComponents.value( className, nullptr );
    if ( groupComponent == nullptr ) {
        qDebug() << "qan::Graph::insertGroup(): Error: Can't find a valid group delegate component.";
        return nullptr;
    }
    qan::Group* group = static_cast< qan::Group* >( createFromDelegate( groupComponent ) );
    if ( group != nullptr ) {
        GTpoGraph::insertGroup( std::shared_ptr<qan::Group>{group} );
        //QQmlEngine::setObjectOwnership( group, QQmlEngine::CppOwnership );

        connect( group, &qan::Group::groupClicked, this, &qan::Graph::groupClicked );
        connect( group, &qan::Group::groupRightClicked, this, &qan::Graph::groupRightClicked );
        connect( group, &qan::Group::groupDoubleClicked, this, &qan::Graph::groupDoubleClicked );
    } else
        qDebug() << "qan::Graph::insertGroup(): Warning: Group creation failed.";

    return group;
=======
qan::Group* Graph::insertGroup()
{
    QQmlComponent* groupComponent{ _groupDelegate.get() };
    if ( groupComponent == nullptr ) {
        qWarning() << "qan::Graph::insertGroup(): Error: Can't find a valid group delegate component.";
        return nullptr;
    }
    auto group = std::make_shared<qan::Group>();
    if ( group ) {
        QQmlEngine::setObjectOwnership( group.get(), QQmlEngine::CppOwnership );
        qan::Style* style = qan::Group::style();
        if ( style != nullptr ) {
            // Group styles are not well supported (for the moment 20170317)
            //_styleManager.setStyleComponent(style, edgeComponent);
            qan::GroupItem* groupItem = static_cast<qan::GroupItem*>( createFromComponent( groupComponent,
                                                                                           *style,
                                                                                           nullptr,
                                                                                           nullptr,                                                                                           group.get() ) );
            if ( groupItem != nullptr ) {
                groupItem->setGroup(group.get());
                groupItem->setGraph(this);
                group->setItem(groupItem);

                GTpoGraph::insertGroup( group );

                auto notifyGroupClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupClicked, notifyGroupClicked );

                auto notifyGroupRightClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupRightClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupRightClicked, notifyGroupRightClicked );

                auto notifyGroupDoubleClicked = [this] (qan::GroupItem* groupItem, QPointF p) {
                    if ( groupItem != nullptr && groupItem->getGroup() != nullptr )
                        emit this->groupDoubleClicked(groupItem->getGroup(), p);
                };
                connect( groupItem, &qan::GroupItem::groupDoubleClicked, notifyGroupDoubleClicked );
            } else
                qWarning() << "qan::Graph::insertGroup(): Warning: Group delegate from QML component creation failed.";
        } else qWarning() << "qan::Graph::insertGroup(): Error: style() factory has returned a nullptr style.";
    }
    return group.get();
>>>>>>> dev
}

void    Graph::removeGroup( qan::Group* group )
{
    if ( group == nullptr )
        return;
    // Reparent all group childrens (ie node) to graph before destructing the group
    // otherwise all child iems get destructed too
<<<<<<< HEAD
    auto& groupNodes{group->getNodes()};
    for ( auto& node : groupNodes ) {
        auto nodePtr{ node.lock() };
        if ( nodePtr != nullptr ) {
            nodePtr->ungroup();
            nodePtr->setParentItem( this );
=======

    // FIXME QAN3       (use std::copy() here...)
    /*auto groupNodes{group->getNodes()};
    for ( auto& node : groupNodes ) {
        auto nodePtr{ node.lock() };
        if ( nodePtr != nullptr ) {
            // FIXME QAN3
            //nodePtr->ungroup();
            //nodePtr->setParentItem( this );
>>>>>>> dev
        }
    }
    WeakGroup weakGroup = group->shared_from_this();
    if ( !weakGroup.expired() )
<<<<<<< HEAD
        gtpo::GenGraph< Config >::removeGroup( weakGroup );
=======
        gtpo::GenGraph< qan::GraphConfig >::removeGroup( weakGroup );
    */
>>>>>>> dev
}

bool    Graph::hasGroup( qan::Group* group ) const
{
    if ( group == nullptr )
        return false;
<<<<<<< HEAD
    WeakGroup weakGroup;
    try {
        weakGroup = WeakGroup{ group->shared_from_this() };
    } catch ( std::bad_weak_ptr ) { return false; }
    return gtpo::GenGraph< qan::Config >::hasGroup( weakGroup );
}
//-----------------------------------------------------------------------------

/* Graph Initialization Management *///----------------------------------------
void    Graph::initializeRandom( int nodeCount,
                                 int   minOutNodes, int maxOutNodes,
                                 qreal minWidth, qreal maxWidth,
                                 qreal minHeight, qreal maxHeight,
                                 QRectF br )
{
    if ( !br.isValid() || br.isEmpty() )
        return;
    RandomGraph::RandomConfig rc;
    rc.nodeClassName = "qan::Node";
    rc.edgeClassName = "qan::Edge";
    rc.nodeCount = nodeCount;
    rc.outNodeRng = std::make_pair( minOutNodes, maxOutNodes );
    rc.widthRng = std::make_pair( minWidth, maxWidth );
    rc.heightRng = std::make_pair( minHeight, maxHeight );
    rc.xRng = std::make_pair( 0, br.width() - maxWidth );
    rc.yRng = std::make_pair( 0, br.height() - maxHeight );
    RandomGraph::generate< qan::Graph >( *this, rc );
    qreal nz{0.};
    for ( auto& node : getNodes() ) {
        node->setZ(nz);   // Force valid z values
        nz += 1.0;
    }
=======
    return GTpoGraph::hasGroup( WeakGroup{group->shared_from_this()} );
}

auto    qan::Graph::groupNode( qan::Group* group, qan::Node* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            GTpoGraph::groupNode( WeakGroup{group->shared_from_this()}, node->shared_from_this() );
        } catch ( ... ) { qWarning() << "qan::Graph::groupNode(): Topology error."; }
        if ( node->getGroup().lock().get() == group &&  // Check that group insertion succeed
             group->getItem() != nullptr &&
             node->getItem() != nullptr ) {
            group->getItem()->groupNodeItem(node->getItem());
        }
    }
}

auto    qan::Graph::groupNode( Group* group, qan::Group* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        qDebug() << "qan::Graph::groupNode(qan::Group*, qan::Group*)";
        try {
            /*if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );*/
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}

auto    qan::Graph::ungroupNode( Group* group, qan::Node* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        qDebug() << "qan::Graph::ungroupNode()";
        try {
            if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}

auto    qan::Graph::ungroupNode( Group* group, qan::Group* node ) noexcept(false) -> void
{
    if ( group != nullptr &&
         node != nullptr ) {
        try {
            /*if ( group->getItem() )
                group->getItem()->ungroupNodeItem(node->getItem());
            GTpoGraph::ungroupNode( group->shared_from_this(), node->shared_from_this() );*/
        } catch ( ... ) { qWarning() << "qan::Graph::ungroupNode(): Topology error."; }
    }
}
//-----------------------------------------------------------------------------


/* Selection Management *///---------------------------------------------------
void    Graph::setSelectionPolicy( SelectionPolicy selectionPolicy ) noexcept
{
    if ( selectionPolicy == _selectionPolicy )  // Binding loop protection
        return;
    _selectionPolicy = selectionPolicy;
    if ( selectionPolicy == SelectionPolicy::NoSelection )
        clearSelection();
    emit selectionPolicyChanged( );
}

void    Graph::setSelectionColor( QColor selectionColor ) noexcept
{
    _selectionColor = selectionColor;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr &&
             node->getItem()->getSelectionItem() != nullptr ) {
            QQuickItem* selectionItem = node->getItem()->getSelectionItem();
            selectionItem->setProperty( "color", QVariant::fromValue( QColor(0,0,0,0) ) );
            QObject* rectangleBorder = selectionItem->property( "border" ).value<QObject*>();
            if ( rectangleBorder != nullptr )
                rectangleBorder->setProperty( "color", selectionColor );
        }
    }
    emit selectionColorChanged();
}

void    Graph::setSelectionWeight( qreal selectionWeight ) noexcept
{
    if ( qFuzzyCompare( selectionWeight, _selectionWeight ) )   // Never 0
        return;
    _selectionWeight = selectionWeight;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->configureSelectionItem( selectionWeight, getSelectionMargin() );
    }
    emit selectionWeightChanged();
}

void    Graph::setSelectionMargin( qreal selectionMargin ) noexcept
{
    if ( qFuzzyCompare( selectionMargin, _selectionMargin ) )   // Never 0
        return;
    _selectionMargin = selectionMargin;
    for ( auto& node : _selectedNodes ) {   // Update visible selection hilight item
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->configureSelectionItem( getSelectionWeight(), selectionMargin );
    }
    emit selectionMarginChanged();
}

template < class Primitive_t >
bool    selectPrimitiveImpl( Primitive_t& primitive,
                         Qt::KeyboardModifiers modifiers,
                         qan::Graph& graph )
{
    if ( graph.getSelectionPolicy() == qan::Graph::SelectionPolicy::NoSelection )
        return false;

    bool selectPrimitive{ false };
    bool ctrlPressed = modifiers & Qt::ControlModifier;
    if ( primitive.getItem() == nullptr )
        return false;

    if ( primitive.getItem()->getSelected() ) {
        if ( ctrlPressed )          // Click on a selected node + CTRL = deselect node
            graph.removeFromSelection( primitive );
    } else {
        switch ( graph.getSelectionPolicy() ) {
        case qan::Graph::SelectionPolicy::SelectOnClick:
            selectPrimitive = true;        // Click on an unselected node with SelectOnClick = select node
            if ( !ctrlPressed )
                graph.clearSelection();
            break;
        case qan::Graph::SelectionPolicy::SelectOnCtrlClick:
            selectPrimitive = ctrlPressed; // Click on an unselected node with CTRL pressed and SelectOnCtrlClick = select node
            break;
        case qan::Graph::SelectionPolicy::NoSelection: break;
        }
    }
    if ( selectPrimitive ) {
        graph.addToSelection( primitive );
        return true;
    }
    return false;
}

bool    Graph::selectNode( qan::Node& node, Qt::KeyboardModifiers modifiers ) { return selectPrimitiveImpl<qan::Node>(node, modifiers, *this); }
bool    Graph::selectGroup( qan::Group& group, Qt::KeyboardModifiers modifiers ) { return selectPrimitiveImpl<qan::Group>(group, modifiers, *this); }

template < class Primitive_t >
void    addToSelectionImpl( Primitive_t& primitive,
                            qcm::ContainerModel< QVector, Primitive_t* >& selectedPrimitives,
                            qan::Graph& graph )
{
    if ( !selectedPrimitives.contains( &primitive ) ) {
        selectedPrimitives.append( &primitive );
        if ( primitive.getItem() != nullptr ) {
            // Eventually, create and configure node item selection item
            if ( primitive.getItem()->getSelectionItem() == nullptr )
                primitive.getItem()->setSelectionItem(graph.createRectangle(primitive.getItem()));
            primitive.getItem()->configureSelectionItem( graph.getSelectionColor(),
                                                         graph.getSelectionWeight(),
                                                         graph.getSelectionMargin() );
            primitive.getItem()->setSelected( true );
        }
    }
}

void    Graph::addToSelection( qan::Node& node ) { addToSelectionImpl<qan::Node>(node, _selectedNodes, *this); }
void    Graph::addToSelection( qan::Group& group ) { addToSelectionImpl<qan::Group>(group, _selectedGroups, *this); }

template < class Primitive_t >
void    removeFromSelectionImpl( Primitive_t& primitive,
                             qcm::ContainerModel< QVector, Primitive_t* >& selectedPrimitives )
{
    if ( selectedPrimitives.contains( &primitive ) )
        selectedPrimitives.remove( &primitive );
    if ( primitive.getItem() != nullptr )
        primitive.getItem()->setSelected( false );
}

void    Graph::removeFromSelection( qan::Node& node ) { removeFromSelectionImpl<qan::Node>(node, _selectedNodes); }
void    Graph::removeFromSelection( qan::Group& group ) { removeFromSelectionImpl<qan::Group>(group, _selectedGroups); }
void    Graph::removeFromSelection( QQuickItem* item ) {
    const auto nodeItem = qobject_cast<qan::NodeItem*>(item);
    if ( nodeItem != nullptr &&
         nodeItem->getNode() != nullptr ) {
        _selectedNodes.remove(nodeItem->getNode());
    } else {
        const auto groupItem = qobject_cast<qan::GroupItem*>(item);
        if ( groupItem != nullptr &&
             groupItem->getGroup() != nullptr )
            _selectedGroups.remove(groupItem->getGroup());
    }
}

void    Graph::clearSelection()
{
    for ( auto node : _selectedNodes )
        if ( node != nullptr &&
             node->getItem() != nullptr )
            node->getItem()->setSelected( false );
    _selectedNodes.clear();
    for ( auto group : _selectedGroups )
        if ( group != nullptr &&
             group->getItem() != nullptr )
            group->getItem()->setSelected( false );
    _selectedGroups.clear();
}

void    Graph::mousePressEvent( QMouseEvent* event )
{
    if ( event->button() == Qt::LeftButton ) {
        clearSelection();
        forceActiveFocus();
    }
    event->ignore();
    qan::GraphConfig::GraphBase::mousePressEvent( event );
>>>>>>> dev
}
//-----------------------------------------------------------------------------

} // ::qan

