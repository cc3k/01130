#include "mainthread.h"

#include "diagnosticobjectlist.h"
#include "visualgrid.h"
#include "visualgroup.h"
#include "visualobject.h"
#include "xmlconfig.h"

#include <QDebug>

MainThread::MainThread()
{
    qDebug() << "MainThread constructor";
}

void MainThread::init()
{
}

QGridLayout *MainThread::getGrid() const
{
    DiagnosticObjectList* objectsList = new DiagnosticObjectList();

    VisualGrid* armRootGrid = new VisualGrid(0, "ARM_ROOT_GRID",  0, 3);
    armRootGrid->setId(1);
    armRootGrid->setParentid(0);
    objectsList->addGrid(armRootGrid);

    VisualGrid* sysGrid = new VisualGrid(0, "SYS_ROOT_GRID");
    sysGrid->setId(2);
    sysGrid->setParentid(0);
    objectsList->addGrid(sysGrid);

    //--------------------------------------------------------

    VisualGrid* armGrid_1 = new VisualGrid(0, "АРМ_CРР_СЕТКА", 1, 1);
    armGrid_1->setId(11);
    armGrid_1->setParentid(1);
    armGrid_1->setPosxy(0, 0);
    objectsList->addGrid(armGrid_1);

    VisualGrid* armGrid_2 = new VisualGrid(0, "АРМ_РТР_СЕТКА", 1, 1);
    armGrid_2->setId(12);
    armGrid_2->setParentid(1);
    armGrid_2->setPosxy(0, 1);
    objectsList->addGrid(armGrid_2);

    VisualGrid* armGrid_3 = new VisualGrid(0, "АРМ_ОЭСВР_СЕТКА", 1, 1);
    armGrid_3->setId(13);
    armGrid_3->setParentid(1);
    armGrid_3->setPosxy(0, 2);
    objectsList->addGrid(armGrid_3);

    VisualGrid* armGrid_4 = new VisualGrid(0, "АРМ_ЛВС_БРК_СЕТКА", 1, 1);
    armGrid_4->setId(14);
    armGrid_4->setParentid(1);
    armGrid_4->setPosxy(0, 3);
    objectsList->addGrid(armGrid_4);

    //--------------------------------------------------------

    VisualGroup* group_121 = new VisualGroup(0, "СТОЙКА 3 - АРМ РТР1");
    group_121->setId(121);
    group_121->setParentid(12);
    group_121->setPosxy(0, 0);
    objectsList->addGroup(group_121);

    VisualGroup* group_122 = new VisualGroup(0, "СТОЙКА 4 - АРМ РТР2");
    group_122->setId(122);
    group_122->setParentid(12);
    group_122->setPosxy(1, 0);
    objectsList->addGroup(group_122);

    VisualGroup* group_123 = new VisualGroup(0, "СТОЙКА 5 - АРМ РТР3");
    group_123->setId(123);
    group_123->setParentid(12);
    group_123->setPosxy(0, 1);
    objectsList->addGroup(group_123);

    VisualGroup* group_124 = new VisualGroup(0, "СТОЙКА 6 - АРМ РТР4");
    group_124->setId(124);
    group_124->setParentid(12);
    group_124->setPosxy(1, 1);
    objectsList->addGroup(group_124);

    VisualGroup* group_131 = new VisualGroup(0, "СТОЙКА СЕРВЕРНАЯ 1");
    group_131->setId(131);
    group_131->setParentid(13);
    group_131->setPosxy(0, 0);
    objectsList->addGroup(group_131);

    VisualObject* object_1211 = new VisualObject(0, "ARMU3U - РТР1");
    object_1211->setId(1211);
    object_1211->setParentid(121);
    object_1211->setPosxy(0, 0);
    objectsList->addObject(object_1211);

    VisualObject* object_1212 = new VisualObject(0, "ARMU3D - РТР1");
    object_1212->setId(1212);
    object_1212->setParentid(121);
    object_1212->setPosxy(1, 0);
    objectsList->addObject(object_1212);

    VisualObject* object_1221 = new VisualObject(0, "ARMU4U - РТР2");
    object_1221->setId(1221);
    object_1221->setParentid(122);
    object_1221->setPosxy(0, 0);
    objectsList->addObject(object_1221);

    VisualObject* object_1222 = new VisualObject(0, "ARMU4D - РТР2");
    object_1222->setId(1222);
    object_1222->setParentid(122);
    object_1222->setPosxy(1, 0);
    objectsList->addObject(object_1222);

    VisualObject* object_1311 = new VisualObject(0, "Верхний вычислитель");
    object_1311->setId(1311);
    object_1311->setParentid(123);
    object_1311->setPosxy(0, 0);
    objectsList->addObject(object_1311);

    VisualObject* object_1312 = new VisualObject(0, "Верхний вычислитель");
    object_1312->setId(1312);
    object_1312->setParentid(124);
    object_1312->setPosxy(0, 0);
    objectsList->addObject(object_1312);


    foreach (VisualGrid* item, objectsList->getGridList()) {
        quint32 parentid = item->getParentid();

        qDebug() << item->getName() << "id:"  << item->getId() << " parentid:" << parentid;
        qDebug() << "posX:" << item->getPosx() << " posY:" << item->getPosy();

        if (((item->getPosx() & item->getPosy()) != 127) & (item->getParentid() != 0))
        {

        }
    }

    qDebug() << "total items: " << objectsList->getItemList().size();
    for (int i = 0; i < objectsList->getItemList().size(); i++)
    {
        //qDebug() << objectsList->getItemList().at(i)->getName() << "with id " << objectsList->getItemList().at(i)->getId();
        if (objectsList->getItemList().at(i)->getParentid() != 0)
        {
            //qDebug() << "parent id: " << objectsList->getItemList().at(i)->getParentid();
            objectsList->getItemById(objectsList->getItemList().at(i)->getParentid())->addItem(objectsList->getItemList().at(i));
            //objectsList->getItemById(objectsList->getItemList().at(i)->getParentid())->addChildId(objectsList->getItemList().at(i)->getId());
        }
        else
        {
            qDebug() << "parentless node";
        }
    }

    //qDebug() << "child for object 121 " << objectsList->getItemById(121)->getChildSet();

    return armRootGrid->getLayout();
}
