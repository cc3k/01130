#include "xmlconfig.h"

XmlConfig::XmlConfig(QString name)
{
    configName = name;
    objectsList = new DiagnosticObjectList;
}

void XmlConfig::writeConfig()
{
    configFile = new QFile(configName);
    if (!configFile->open(QIODevice::WriteOnly)) return;

       xmlWriter = new QXmlStreamWriter;
       xmlWriter->setDevice(configFile);

       xmlWriter->setAutoFormatting(true);
       xmlWriter->writeStartDocument();

       xmlWriter->writeStartElement("configuration");

       //--------------------------------------------

       // grid
       xmlWriter->writeComment("grid section");
       for (int i = 0; i < objectsList->getGridList().size(); i++)
       {
            writeGrid(xmlWriter, objectsList->getGridList().at(i));
       }
       //xmlWriter->writeComment("end grid section");

       // group
       xmlWriter->writeComment("group section");
       for (int i = 0; i < objectsList->getGroupList().size(); i++)
       {
            writeGroup(xmlWriter, objectsList->getGroupList().at(i));
       }
       //xmlWriter->writeComment("end group section");

       // object
       xmlWriter->writeComment("diagnostic objects section");
       for (int i = 0; i < objectsList->getObjectList().size(); i++)
       {
           writeObject(xmlWriter, objectsList->getObjectList().at(i));
       }
       //xmlWriter->writeComment("end diagnostic objects section");

       // diagnostic item
       xmlWriter->writeComment("diagnostic items section");
       for (int i = 0; i < itemList.size(); i++)
       {
           writeItem(xmlWriter, itemList.at(i));
       }
       //xmlWriter->writeComment("end diagnostic items section");

       //--------------------------------------------

       xmlWriter->writeEndElement();
       xmlWriter->writeEndDocument();

       configFile->close();

       delete configFile;
       delete xmlWriter;

}

void XmlConfig::readConfig()
{
    itemList.clear();
    objectsList->clear();

    configFile = new QFile(configName);
    if (!configFile->open(QIODevice::ReadOnly))
       {
           return;
       }

       xmlReader = new QXmlStreamReader;
       xmlReader->setDevice(configFile);

       while (!xmlReader->atEnd() && !xmlReader->hasError())
           {
               QXmlStreamReader::TokenType token = xmlReader->readNext();
               if (token == QXmlStreamReader::StartDocument)
               {
                   continue;
               }
               if (token == QXmlStreamReader::StartElement)
               {
                   if (xmlReader->name() == "configuration")
                   {
                       //qDebug() << "configuration";
                   }

                   if (xmlReader->name() == "grid")
                   {
                       QString name = xmlReader->attributes().value("name").toString();
                       VisualGrid *grid = new VisualGrid(0, name);
                       grid->setId(xmlReader->attributes().value("id").toInt());
                       grid->setParentId(xmlReader->attributes().value("parentid").toInt());
                       grid->setPosXY(xmlReader->attributes().value("posx").toInt(), xmlReader->attributes().value("posy").toInt());
                       //qDebug() << grid->getName() << grid->getId() << grid->getParentId() << grid->getPosx() << grid->getPosy();
                       objectsList->addGrid(grid);
                   }

                   if (xmlReader->name() == "group")
                   {
                       QString name = xmlReader->attributes().value("name").toString();
                       VisualGroup *group = new VisualGroup(0, name);
                       group->setId(xmlReader->attributes().value("id").toInt());
                       group->setParentId(xmlReader->attributes().value("parentid").toInt());
                       group->setPosXY(xmlReader->attributes().value("posx").toInt(), xmlReader->attributes().value("posy").toInt());

                       quint16 align = xmlReader->attributes().value("alignment").toInt();
                       if (align == 1)
                       {
                           group->setAlignment(Qt::AlignLeft);
                       }
                       if (align == 2)
                       {
                           group->setAlignment(Qt::AlignRight);
                       }
                       if (align == 132)
                       {
                           group->setAlignment(Qt::AlignCenter);
                       }

                       QString offcheck = xmlReader->attributes().value("offcheck").toString();
                       if (offcheck == "true")
                       {
                           group->setNone();
                       }

                       objectsList->addGroup(group);
                   }

                   if (xmlReader->name() == "object")
                   {
                       QString name = xmlReader->attributes().value("name").toString();
                       VisualObject *object = new VisualObject(0, name);
                       object->setId(xmlReader->attributes().value("id").toInt());
                       object->setParentId(xmlReader->attributes().value("parentid").toInt());
                       object->setPosXY(xmlReader->attributes().value("posx").toInt(), xmlReader->attributes().value("posy").toInt());
                       objectsList->addObject(object);

                   }

                   if (xmlReader->name() == "item")
                   {
                       QString name = xmlReader->attributes().value("name").toString();
                       DiagnosticItem *item = new DiagnosticItem(0, name);
                       item->setId(xmlReader->attributes().value("id").toInt());
                       item->setParentId(xmlReader->attributes().value("parentid").toInt());
                       item->setHsys(xmlReader->attributes().value("hsys").toInt());
                       item->setHsubSys(xmlReader->attributes().value("hsubsys").toInt());
                       item->setSys(xmlReader->attributes().value("sys").toInt());
                       item->setSubSys(xmlReader->attributes().value("subsys").toInt());
                       item->setIp(xmlReader->attributes().value("ip").toString());
                       if (xmlReader->attributes().value("state").toString() == "offcheck")
                       {
                           item->setState(Global::State::offcheck);
                       }
                       itemList.append(item);
                   }
               }
           }

}

void XmlConfig::setItemList(const QList<DiagnosticItem *> &value)
{
    itemList = value;
}

void XmlConfig::setObjectsList(DiagnosticObjectList *value)
{
    objectsList = value;
}

QList<DiagnosticItem *> XmlConfig::getItemList() const
{
    return itemList;
}

DiagnosticObjectList *XmlConfig::getObjectsList() const
{
    return objectsList;
}

VisualGrid *XmlConfig::getRootGrid()
{
    for (int i = 0; i < objectsList->getGridList().size(); i++)
    {
        if (objectsList->getGridList().at(i)->getId() == 1)
        {
            //qDebug() << objectsList->getGridList().at(i)->getName();
            return objectsList->getGridList().at(i);
        }
    }

    return Q_NULLPTR;
}

void XmlConfig::writeItem(QXmlStreamWriter *writer, const DiagnosticItem *item)
{
    writer->writeStartElement("item");
    writer->writeAttribute("name", item->getName());
    writer->writeAttribute("id", QString::number(item->getId()));
    writer->writeAttribute("parentid", QString::number(item->getParentId()));
    writer->writeAttribute("hsys", QString::number(item->getHsys()));
    writer->writeAttribute("hsubsys", QString::number(item->getHsubSys()));
    writer->writeAttribute("sys", QString::number(item->getSys()));
    writer->writeAttribute("subsys", QString::number(item->getSubSys()));
    if (item->getIp() != "")
    {
        writer->writeAttribute("ip", item->getIp());
    }
    if (item->getState() == Global::State::offcheck)
    {
        writer->writeAttribute("state", "offcheck");
    }
    writer->writeEndElement();
}

void XmlConfig::writeGrid(QXmlStreamWriter *writer, const VisualGrid *grid)
{
    writer->writeStartElement("grid");
    writer->writeAttribute("name", grid->getName());
    writer->writeAttribute("id", QString::number(grid->getId()));
    writer->writeAttribute("parentid", QString::number(grid->getParentId()));
    writer->writeAttribute("posx", QString::number(grid->getPosx()));
    writer->writeAttribute("posy", QString::number(grid->getPosy()));
    writer->writeEndElement();
}

void XmlConfig::writeGroup(QXmlStreamWriter *writer, const VisualGroup *group)
{
    writer->writeStartElement("group");
    writer->writeAttribute("name", group->getName());
    writer->writeAttribute("id", QString::number(group->getId()));
    writer->writeAttribute("parentid", QString::number(group->getParentId()));
    writer->writeAttribute("posx", QString::number(group->getPosx()));
    writer->writeAttribute("posy", QString::number(group->getPosy()));
    writer->writeAttribute("offcheck", group->getNone());
    writer->writeAttribute("alignment", QString::number(group->getAlignment()));
    writer->writeEndElement();
}

void XmlConfig::writeObject(QXmlStreamWriter *writer, const VisualObject *object)
{

    writer->writeStartElement("object");
    writer->writeAttribute("name", object->getName());
    writer->writeAttribute("id", QString::number(object->getId()));
    writer->writeAttribute("parentid", QString::number(object->getParentId()));
    writer->writeAttribute("posx", QString::number(object->getPosx()));
    writer->writeAttribute("posy", QString::number(object->getPosy()));
    writer->writeEndElement();
}
