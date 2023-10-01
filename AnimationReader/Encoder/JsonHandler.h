#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "AnimDefinitions/AnimDefHandler.h"
#pragma once

class JSONHandler {

    public:
    JSONHandler(QString filePath){
        this->path = filePath;
        QFile file(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        this->document = QJsonDocument::fromJson(file.readAll()); file.close();
        objects = this->document.array();
    }

    void writeKeytoJSON(StateNode::KeyValueProp prop,QJsonObject* obj){
        QString key = QString::number(prop.streamKey);
        switch (ntohl(prop.streamKey)) {
            case (0x46C0C6BF):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0xA8FA930E):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x4D245F2B):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x6782648B):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0xDF4B0B8E):
                obj->insert(key,QString::fromStdString(prop.chars));
                break;
            case (0x60C09DC4):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0xA6D0A4EB):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x5D651EEF):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x4425DADA):
                 obj->insert(key,QString::number(prop.val32));
                break;
            case (0x45991D01):
                obj->insert(key,QString::fromStdString(prop.chars));
                break;
            case (0x48DE1308):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x73EDDE33):
                obj->insert(key,QString::number(prop.val64));
                break;
            case (0x0C3F823A):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x654680ED):
                 obj->insert(key,QString::number(prop.val32));
                break;
            case (0xC248BCF4 ):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
            case (0x8AE38AFE):
                 obj->insert(key,QString::number(prop.val32));
                break;
            case (0x8E0B4BDF):
                obj->insert(key,QString::number(prop.val64));
                break;
            case (0xBE83F7FD):
                obj->insert(key,QString::fromStdString(prop.chars));
                break;
            case (0x78D548C7):
                 obj->insert(key,QString::number(prop.val32));
                break;
            case (0xC8B22C04):
                obj->insert(key,QString::number(prop.valFloat));
                break;
            case (0x19981A6F):
                obj->insert(key,QString::number(prop.valFloat));
                break;
            case (0x9E3F108C):
                obj->insert(key,QString::fromStdString(prop.chars));
                break;
            case (0x4C980DCF):
                obj->insert(key,QString::number(prop.valFloat));
                break;
            case (0xE39D3DA5):
                obj->insert(key,QString::number(prop.valFloat));
                break;
            case (0x0A2E3AF4):
                obj->insert(key, prop.flag ? "true" : "false");
                break;
        }
    }

    void writeNode(StateNode::Node* state, QJsonObject* parent){
        parent->insert("type",QString::number(state->nodeType));
        for (auto &key : state->keyValueProperties )
            writeKeytoJSON(key,parent);
        for (auto &child : state->childNodes ){
            QJsonObject item;
            writeNode(&child,&item);
            parent->insert("Child",item);  }
    }

    void writeStateNodes(StateNode::Definition* def,QJsonObject* parent){
        QJsonObject object;
        for (auto &stateNode : def->stateNodes )
            writeNode(&stateNode, &object);
        parent->insert("Node",object);
    }

    void AppendNodeToJSON(StateNode::Definition def , QString tName){
        // Iterate over the top-level JSON values and look for an object with the desired name parameter
        QJsonArray jsonArray = this->document.array();
        QJsonObject root;

        root.insert("Template Name", tName);
        root.insert("Definition Name", "Dummy");
        writeStateNodes(&def,&root);

        objects.append(root);
    }

    void WriteToFile(){
       this->document.setArray(objects);
       QFile file(path);
       file.open(QIODevice::WriteOnly | QIODevice::Text);
       file.write(this->document.toJson());
       file.close();
    }

private:
    QString path;
    QJsonArray objects;
    QJsonDocument document;
};



#endif // JSONHANDLER_H
