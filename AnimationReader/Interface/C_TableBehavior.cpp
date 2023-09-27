#include "C_TableBehavior.h"
#include <sstream>

void FilterHeaderString(QString* string){
    string->remove("\"");
    string->remove(" :");
    string->remove(" ");
}

int FindPropertyIndex(std::vector<KeyValueProp> properties, uint32_t key){
    for (int i = 0; i < properties.size(); i++)
        if (properties[i].streamKey == key)
            return i;
    return -1;
}
uint32_t HexStringToUInt32(const std::string& hexString) {
    std::istringstream stream(hexString);
    uint32_t result = 0;
    stream >> std::hex >> result;
    return result;
}

void UpdateKeyProperty(StateNode::Node* node, uint32_t key, QString value, QTableWidgetItem* item){
    std::vector<KeyValueProp> properties = node->keyValueProperties;
    int index = FindPropertyIndex(properties,key);
    if(index == -1){ qDebug() << "No Key Found."; return;}
    switch(ntohl(key)){
        case(0xBE83F7FD):
            properties[index].chars = value.toStdString();
            break;
        default:
            break;
    }
    node->keyValueProperties = properties;
}
void CTableBehavior::UpdateNode(DefsTreeWidgetItem* item,
                                QTableWidgetItem* tableItem, QString header){
    FilterHeaderString(&header);
    qDebug() << "\nHeader: " << header;
    uint32_t streamKey = HexStringToUInt32(header.toStdString());
    StateNode::Node* node = item->getStateNode();
    UpdateKeyProperty(node, streamKey, tableItem->text(),tableItem);
}
void CTableBehavior::UpdateState(DefsTreeWidgetItem* item,
                                 QTableWidgetItem* tableItem, QString header){

}
void CTableBehavior::UpdateMember(DefsTreeWidgetItem* item,
                                  QTableWidgetItem* tableItem, QString header){

}
void CTableBehavior::UpdateEvent(DefsTreeWidgetItem *item,
                                 QTableWidgetItem* tableItem, QString header){

}
void CTableBehavior::UpdateFrame(DefsTreeWidgetItem* item,
                                 QTableWidgetItem* tableItem, QString header){

}
void CTableBehavior::UpdateCandidate(DefsTreeWidgetItem* item,
                                     QTableWidgetItem* tableItem, QString header){

}
void CTableBehavior::UpdateTableWithNode(DefsTreeWidgetItem* item,
                                         QTableWidgetItem* tableItem, uint32_t type, QString header)
{
    switch(type){
        case(STAT):
            break;
        case(NODE):
            UpdateNode(item,tableItem,header);
            break;
        case(SYNC):
            break;
        case(DTT_):
            break;
        case(TOVR):
            break;
        case(DESC):
            break;
        case(EVNT):
            break;
        case(GRP_):
            break;
        case(MEMB):
            break;
        case(SELS):
            break;
        case(FRMS):
            break;
        case(CAND):
            break;
        default:
            break;
    }
}

