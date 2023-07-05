/**
 * @file    CSearchResultModel.cpp
 * @author  vgoncharuk
 * @brief   Implementation of the CSearchResultModel class
 */

#include "CSearchResultModel.hpp"
#include "components/log/api/CLog.hpp"
#include "components/settings/api/ISettingsManager.hpp"
#include "components/logsWrapper/api/IMsgWrapper.hpp"
#include "components/logsWrapper/api/IFileWrapper.hpp"

#include "DMA_Plantuml.hpp"

CSearchResultModel::CSearchResultModel(const tSettingsManagerPtr& pSettingsManager, QObject *):
CSettingsManagerClient(pSettingsManager),
mFoundMatchesPack(),
mpFile(nullptr)
{

}

void CSearchResultModel::setFile(const tFileWrapperPtr& pFile)
{
    mpFile = pFile;
}

void CSearchResultModel::updateView(const int& fromRow)
{
    emit dataChanged( index(fromRow,0), index(static_cast<int>(mFoundMatchesPack.matchedItemVec.size() - 1), columnCount(QModelIndex()) - 1) );
    emit layoutChanged();
}

void CSearchResultModel::resetData()
{
    beginResetModel();
    mFoundMatchesPack.matchedItemVec.clear();
    endResetModel();
    updateView();
}

int CSearchResultModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(mFoundMatchesPack.matchedItemVec.size());
}

int CSearchResultModel::columnCount(const QModelIndex &) const
{
    return static_cast<int>(eSearchResultColumn::Last);
}

const tFoundMatchesPackItem& CSearchResultModel::getFoundMatchesItemPack( const QModelIndex& modelIndex ) const
{
    if ( ( modelIndex.row() < 0 || modelIndex.row() >= static_cast<int>(mFoundMatchesPack.matchedItemVec.size()) ) ||
         ( modelIndex.column() < 0 || modelIndex.column() >= static_cast<int>(eSearchResultColumn::Last) ) )
    {
        static const tFoundMatchesPackItem sDummyValue;
        return sDummyValue;
    }

    return mFoundMatchesPack.matchedItemVec[ static_cast<std::size_t>(modelIndex.row()) ];
}

QVariant CSearchResultModel::data(const QModelIndex &index, int role) const
{
    if ( ( index.row() < 0 || index.row() >= static_cast<int>(mFoundMatchesPack.matchedItemVec.size()) ) ||
         ( index.column() < 0 || index.column() >= static_cast<int>(eSearchResultColumn::Last) ) )
        return QVariant();

    QVariant result;

    auto column = static_cast<eSearchResultColumn>(index.column());

    if (role == Qt::TextAlignmentRole)
    {
        Qt::AlignmentFlag alignment = Qt::AlignCenter;

        switch( static_cast<eSearchResultColumn>(index.column()) )
        {
            case eSearchResultColumn::UML_Applicability: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Index: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Time: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Timestamp: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Count: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Ecuid: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Apid: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Ctid: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::SessionId: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Type: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Subtype: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Mode: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Args: { alignment = Qt::AlignCenter; } break;
            case eSearchResultColumn::Payload: { alignment = static_cast<Qt::AlignmentFlag>((Qt::AlignLeft | Qt::AlignVCenter).operator QFlags<Qt::AlignmentFlag>::Int()); } break;
            case eSearchResultColumn::Last: { alignment = Qt::AlignCenter; } break;
        }

        result = alignment;
    }
    else if ( role == Qt::DisplayRole && column != eSearchResultColumn::UML_Applicability )
    {
        auto msgId = mFoundMatchesPack.matchedItemVec[static_cast<std::size_t>(index.row())].getItemMetadata().msgId;
        const auto& pMsg = mpFile->getMsg(msgId);
        auto pStrValue = getDataStrFromMsg(msgId, pMsg, column);

        if(nullptr != pStrValue)
        {
            result = std::move(*pStrValue);
        }
    }
    else if( role == Qt::CheckStateRole && ( index.column() == static_cast<int>(eSearchResultColumn::UML_Applicability) ) )
    {
        const auto& UMLInfo = mFoundMatchesPack.matchedItemVec[static_cast<std::size_t>(index.row())].getItemMetadata().UMLInfo;

        if(true == UMLInfo.bUMLConstraintsFulfilled)
        {
            if(true == UMLInfo.bApplyForUMLCreation)
            {
                result = Qt::CheckState::Checked;
            }
            else
            {
                result = Qt::CheckState::Unchecked;
            }
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        if(index.column() == static_cast<int>(eSearchResultColumn::UML_Applicability))
        {
            result = Qt::AlignLeft;
        }
    }

    return result;
}

QVariant CSearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = getName(static_cast<eSearchResultColumn>(section));
    }

    return result;
}

Qt::ItemFlags CSearchResultModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result;

    if (false == index.isValid())
    {
        result = Qt::NoItemFlags;
    }
    else
    {
        if(static_cast<eSearchResultColumn>(index.column()) == eSearchResultColumn::UML_Applicability)
        {
            const auto& UMLInfo = mFoundMatchesPack.matchedItemVec[static_cast<std::size_t>(index.row())].getItemMetadata().UMLInfo;

            if(true == UMLInfo.bUMLConstraintsFulfilled)
            {
                result = QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
            }
            else
            {
                result = QAbstractItemModel::flags(index) & (~Qt::ItemIsEditable);
            }
        }
        else
        {
            result = QAbstractItemModel::flags(index);
        }
    }

    return result;
}

std::pair<bool, tIntRange> CSearchResultModel::addNextMessageIdxVec(const tFoundMatchesPack &foundMatchesPack)
{
    std::pair<bool, tIntRange> result;

    if(false == foundMatchesPack.matchedItemVec.empty())
    {
        beginInsertRows(QModelIndex(), static_cast<int>(mFoundMatchesPack.matchedItemVec.size()),
                        static_cast<int>(mFoundMatchesPack.matchedItemVec.size() + foundMatchesPack.matchedItemVec.size() - 1));
        result.second.from = static_cast<int>(mFoundMatchesPack.matchedItemVec.size());
        mFoundMatchesPack.matchedItemVec.insert(mFoundMatchesPack.matchedItemVec.end(),
                                                foundMatchesPack.matchedItemVec.begin(),
                                                foundMatchesPack.matchedItemVec.end());
        result.second.to = static_cast<int>(mFoundMatchesPack.matchedItemVec.size() - 1);

        endInsertRows();

        updateView(result.second.from);

        result.first = true;
    }

    return result;
}

int CSearchResultModel::getFileIdx( const QModelIndex& idx ) const
{
   int result = -1;

   auto row = idx.row();

   if(row >= 0 && static_cast<size_t>(row) < mFoundMatchesPack.matchedItemVec.size())
   {
       result = mFoundMatchesPack.matchedItemVec[static_cast<std::size_t>(row)].getItemMetadata().msgIdFiltered;
   }

   return result;
}

QString CSearchResultModel::getStrValue(const int& row, const eSearchResultColumn& column) const
{
    QString result;

    if(column != eSearchResultColumn::UML_Applicability)
    {
        auto index = createIndex(row, static_cast<int>(column));

        if(true == index.isValid())
        {
            result = index.data().value<QString>();
        }
    }

    return result;
}

std::pair<int /*rowNumber*/, QString /*diagramContent*/> CSearchResultModel::getUMLDiagramContent() const
{
    std::pair<int, QString> result;

    int& numberOfRows = result.first;
    QString& outputString = result.second;

    outputString.append("@startuml\n");

    if(true == getSettingsManager()->getUML_Autonumber())
    {
        outputString.append("autonumber\n");
    }

    outputString.append("skinparam backgroundColor white\n");

#ifdef __linux__
    outputString.append("skinparam defaultFontName Ubuntu Mono\n");
#elif _WIN32
    outputString.append("skinparam defaultFontName monospaced\n");
#else
    outputString.append("skinparam defaultFontName monospaced\n");
#endif

    //let's represent wheether UML data is properly filled in
    int row = 0;

    for(const auto& foundMatchPack : mFoundMatchesPack.matchedItemVec)
    {
        QString subStr;

        const auto& itemMetadata = foundMatchPack.getItemMetadata();

        if(true == itemMetadata.UMLInfo.bUMLConstraintsFulfilled
           && true == itemMetadata.UMLInfo.bApplyForUMLCreation)
        {
            // Result string - <UCL> <URT|URS|UE> <US> : [timestamp] <USID><UM>(<UA>)

            tIntRange insertMethodFormattingRange;
            tIntRange insertMethodFormattingOffset;

            auto getUMLItemRepresentation = [this, &itemMetadata, &row](const eUML_ID& UML_ID)->std::pair<bool, QString>
            {
                std::pair<bool, QString> UMLRepresentationResult;
                UMLRepresentationResult.first = false;

                auto foundUMLDataItem = itemMetadata.UMLInfo.UMLDataMap.find(UML_ID);

                if(foundUMLDataItem != itemMetadata.UMLInfo.UMLDataMap.end())
                {
                    for(const auto& item : foundUMLDataItem->second)
                    {
                        if(true == item.UML_Custom_Value.isEmpty()) // if there is no client-defined value
                        {
                            // let's use value from the corresponding group
                            for(const auto& stringCoverageMapItem : item.stringCoverageMap)
                            {
                                auto column = stringCoverageMapItem.first;

                                QString message = getStrValue(row, column);

                                auto messageSize = message.size();
                                const auto& range = stringCoverageMapItem.second.range;

                                if(range.from >= 0 && range.from < messageSize &&
                                   range.to >= 0 && range.to < messageSize )
                                {
                                    switch(UML_ID)
                                    {
                                        case eUML_ID::UML_REQUEST:
                                            UMLRepresentationResult.second.append("->");
                                            break;
                                        case eUML_ID::UML_RESPONSE:
                                        case eUML_ID::UML_EVENT:
                                            UMLRepresentationResult.second.append("<-");
                                            break;
                                        case eUML_ID::UML_ARGUMENTS:
                                        {
                                            int numberOfCharacters = range.to - range.from + 1;
                                            QString argString = message.mid(range.from, numberOfCharacters);
                                            argString.replace("[[", "[ [");
                                            argString.replace("]]", "] ]");
                                            UMLRepresentationResult.second.append(argString);

                                            if(true == stringCoverageMapItem.second.bAddSeparator)
                                            {
                                                UMLRepresentationResult.second.append(" ");
                                            }
                                        }
                                            break;
                                        case eUML_ID::UML_CLIENT:
                                        case eUML_ID::UML_SERVICE:
                                        {
                                            QString str;
                                            str.append("\"");
                                            str.append(message.mid(range.from, range.to - range.from + 1));

                                            if(true == stringCoverageMapItem.second.bAddSeparator)
                                            {
                                                str.append(" ");
                                            }

                                            str.append("\"");

                                            UMLRepresentationResult.second.append(str);
                                        }
                                            break;
                                        case eUML_ID::UML_TIMESTAMP:
                                        {
                                            QString str;
                                            str.append("[");
                                            str.append( message.mid(range.from, range.to - range.from + 1) );
                                            str.append("] ");

                                            UMLRepresentationResult.second.append(str);
                                        }
                                        break;
                                        default:
                                        {
                                            UMLRepresentationResult.second.append(message.mid(range.from, range.to - range.from + 1));

                                            if(true == stringCoverageMapItem.second.bAddSeparator)
                                            {
                                                UMLRepresentationResult.second.append(" ");
                                            }
                                        }
                                            break;
                                    }
                                }

                                UMLRepresentationResult.first = true;
                            }
                        }
                        else // otherwise
                        {
                            switch(UML_ID)
                            {
                                case eUML_ID::UML_CLIENT:
                                case eUML_ID::UML_SERVICE:
                                {
                                    QString str;
                                    str.reserve(item.UML_Custom_Value.size() + 2);
                                    str.append("\"");
                                    str.append(item.UML_Custom_Value);
                                    str.append("\"");

                                    // let's directly use client-defined value, ignoring value from the group
                                    UMLRepresentationResult.second.append(str);
                                }
                                    break;
                                case eUML_ID::UML_TIMESTAMP:
                                {
                                    const auto column = eSearchResultColumn::Timestamp;
                                    QString timestampVal = getStrValue(row, column);
                                    QString str;
                                    str.reserve(timestampVal.size() + 3);
                                    str.append("[");
                                    str.append( timestampVal );
                                    str.append("] ");

                                    // let's use dlt's native timestamp
                                    UMLRepresentationResult.second.append(str);
                                }
                                    break;
                                default:
                                {
                                    // let's directly use client-defined value, ignoring value from the group
                                    UMLRepresentationResult.second.append(item.UML_Custom_Value);
                                }
                                    break;
                            }

                            UMLRepresentationResult.first = true;
                        }
                    }
                }
                else
                {
                    if(UML_ID == eUML_ID::UML_TIMESTAMP)
                    {
                        const auto column = eSearchResultColumn::Timestamp;
                        QString timestampVal = getStrValue(row, column);
                        QString str;
                        str.reserve(timestampVal.size() + 3);
                        str.append("[");
                        str.append( timestampVal );
                        str.append("] ");

                        // let's use dlt's native timestamp
                        UMLRepresentationResult.second.append(str);
                        UMLRepresentationResult.first = true;
                    }
                }

                return UMLRepresentationResult;
            };

            auto appendUMLData = [&getUMLItemRepresentation, &subStr](const eUML_ID& UML_ID)
            {
                auto findUCLResult = getUMLItemRepresentation(UML_ID);

                if(true == findUCLResult.first)
                {
                    subStr.append(findUCLResult.second);
                }
                else
                {
                    //SEND_ERR(QString("Was not able to find \"%1\" field!").arg(getUMLIDAsString(UML_ID)));
                }
            };

            appendUMLData(eUML_ID::UML_CLIENT);
            subStr.append(" ");
            appendUMLData(eUML_ID::UML_REQUEST);
            appendUMLData(eUML_ID::UML_RESPONSE);
            appendUMLData(eUML_ID::UML_EVENT);
            subStr.append(" ");
            appendUMLData(eUML_ID::UML_SERVICE);
            subStr.append(" : ");
            int wrappingStartingPoint = subStr.size();
            appendUMLData(eUML_ID::UML_TIMESTAMP);
            appendUMLData(eUML_ID::UML_SEQUENCE_ID);
            subStr.append(" ");
            insertMethodFormattingRange.from = subStr.size();
            appendUMLData(eUML_ID::UML_METHOD);
            insertMethodFormattingRange.to = subStr.size();
            subStr.append("(");

            if(true == getSettingsManager()->getUML_ShowArguments())
            {
                appendUMLData(eUML_ID::UML_ARGUMENTS);
            }
            else
            {
                subStr.append(" ... ");
            }

            subStr.append(")");

            subStr.append("\n");

            // wrapping logic
            {
                if(true == getSettingsManager()->getUML_WrapOutput())
                {
                    const int insertNewLineRange = 100;

                    if(subStr.size() > insertNewLineRange)
                    {
                        const QString insertStr = "\\n";

                        subStr.reserve(subStr.size() + ( insertStr.size() * ( (subStr.size() / insertNewLineRange) + 1 ) ) );

                        int currentIndex = wrappingStartingPoint + insertNewLineRange;
                        int currentOffset = 0;

                        while( (currentIndex + currentOffset) < subStr.size())
                        {
                            if(currentIndex + currentOffset < insertMethodFormattingRange.from)
                            {
                                insertMethodFormattingOffset.from += insertStr.size();
                            }

                            if(currentIndex + currentOffset < insertMethodFormattingRange.to)
                            {
                                insertMethodFormattingOffset.to += insertStr.size();
                            }

                            subStr.insert( currentIndex + currentOffset, insertStr );
                            currentIndex += insertNewLineRange;
                            currentOffset += insertStr.size();
                        }
                    }
                }
            }

            subStr.insert(insertMethodFormattingRange.from + insertMethodFormattingOffset.from, "__**");
            subStr.insert(insertMethodFormattingRange.to + insertMethodFormattingOffset.to + 4, "**__");

            outputString.append(subStr);

            ++numberOfRows;

            const auto& maxRowsNumber = getSettingsManager()->getUML_MaxNumberOfRowsInDiagram();

            // if we've reached the limit
            if(numberOfRows >= maxRowsNumber)
            {
                // stop addition of new rows
                SEND_WRN(QString("Not all UML content will be rendered. Number of rows in diagram was limited to %1 rows due to specified settings").arg(maxRowsNumber));
                break;
            }
        }

        ++row;
    }

    outputString.append("@enduml");

    return result;
}

void CSearchResultModel::setUML_Applicability( const QModelIndex& index, bool checked )
{
    if(true == index.isValid())
    {
        auto& UML_Info = mFoundMatchesPack.matchedItemVec[static_cast<std::size_t>(index.row())].getItemMetadataWriteable().UMLInfo;

        if(true == UML_Info.bUMLConstraintsFulfilled)
        {
            UML_Info.bApplyForUMLCreation = checked;
            dataChanged(index, index);
        }
    }
}

PUML_PACKAGE_BEGIN(DMA_SearchView)
    PUML_CLASS_BEGIN_CHECKED(CSearchResultModel)
        PUML_INHERITANCE_CHECKED(QAbstractTableModel, implements)
        PUML_INHERITANCE_CHECKED(ISearchResultModel, implements)
        PUML_INHERITANCE_CHECKED(CSettingsManagerClient, extends)
        PUML_AGGREGATION_DEPENDENCY_CHECKED(IFileWrapper, 1, 1, uses)
    PUML_CLASS_END()
PUML_PACKAGE_END()
