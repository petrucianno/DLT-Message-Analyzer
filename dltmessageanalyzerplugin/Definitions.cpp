/**
 * @file    Definitions.cpp
 * @author  vgoncharuk
 * @brief   Implementation of the Definitions class
 */

#include <assert.h>

#include <map>
#include <vector>
#include <atomic>

#include <QElapsedTimer>

#include "Definitions.hpp"

#include "QRegularExpression"

#include "CConsoleCtrl.hpp"
#include "CTreeItem.hpp"

Q_DECLARE_METATYPE(eRequestState)
Q_DECLARE_METATYPE(int8_t)
Q_DECLARE_METATYPE(tRequestId)

const QString sDefaultStatusText = "No error ...";
const QString sDefaultRegexFileName = "Regex_Default.json";
const QString sRegexScriptingDelimiter = "_and_";
const QString sRGBPrefix = "RGB_";
const QString sVARPrefix = "VAR_";

const tMsgId INVALID_MSG_ID = -1;
const tRequestId INVALID_REQUEST_ID = static_cast<uint>(-1);

const static std::map<QString, QColor> sColorsMap =
{
{"ok", QColor(0,150,0)},
{"warning", QColor(150,150,0)},
{"error", QColor(150,0,0)},
{"black", QColor(0,0,0)},
{"white", QColor(255,255,255)},
{"red", QColor(255,0,0)},
{"lime", QColor(0,255,0)},
{"blue", QColor(0,0,255)},
{"yellow", QColor(255,255,0)},
{"cyan", QColor(0,255,255)},
{"magenta", QColor(255,0,255)},
{"silver", QColor(192,192,192)},
{"gray", QColor(128,128,128)},
{"maroon", QColor(128,0,0)},
{"olive", QColor(128,128,0)},
{"green", QColor(0,128,0)},
{"purple", QColor(128,0,128)},
{"teal", QColor(0,128,128)},
{"navy", QColor(0,0,128)},
{"maroon", QColor(128,0,0)},
{"dark red", QColor(139,0,0)},
{"brown", QColor(165,42,42)},
{"firebrick", QColor(178,34,34)},
{"crimson", QColor(220,20,60)},
{"red", QColor(255,0,0)},
{"tomato", QColor(255,99,71)},
{"coral", QColor(255,127,80)},
{"indian red", QColor(205,92,92)},
{"light coral", QColor(240,128,128)},
{"dark salmon", QColor(233,150,122)},
{"salmon", QColor(250,128,114)},
{"light salmon", QColor(255,160,122)},
{"orange red", QColor(255,69,0)},
{"dark orange", QColor(255,140,0)},
{"orange", QColor(255,165,0)},
{"gold", QColor(255,215,0)},
{"dark golden rod", QColor(184,134,11)},
{"golden rod", QColor(218,165,32)},
{"pale golden rod", QColor(238,232,170)},
{"dark khaki", QColor(189,183,107)},
{"khaki", QColor(240,230,140)},
{"olive", QColor(128,128,0)},
{"yellow", QColor(255,255,0)},
{"yellow green", QColor(154,205,50)},
{"dark olive green", QColor(85,107,47)},
{"olive drab", QColor(107,142,35)},
{"lawn green", QColor(124,252,0)},
{"chart reuse", QColor(127,255,0)},
{"green yellow", QColor(173,255,47)},
{"dark green", QColor(0,100,0)},
{"green", QColor(0,128,0)},
{"forest green", QColor(34,139,34)},
{"lime", QColor(0,255,0)},
{"lime green", QColor(50,205,50)},
{"light green", QColor(144,238,144)},
{"pale green", QColor(152,251,152)},
{"dark sea green", QColor(143,188,143)},
{"medium spring green", QColor(0,250,154)},
{"spring green", QColor(0,255,127)},
{"sea green", QColor(46,139,87)},
{"medium aqua marine", QColor(102,205,170)},
{"medium sea green", QColor(60,179,113)},
{"light sea green", QColor(32,178,170)},
{"dark slate gray", QColor(47,79,79)},
{"teal", QColor(0,128,128)},
{"dark cyan", QColor(0,139,139)},
{"aqua", QColor(0,255,255)},
{"cyan", QColor(0,255,255)},
{"light cyan", QColor(224,255,255)},
{"dark turquoise", QColor(0,206,209)},
{"turquoise", QColor(64,224,208)},
{"medium turquoise", QColor(72,209,204)},
{"pale turquoise", QColor(175,238,238)},
{"aqua marine", QColor(127,255,212)},
{"powder blue", QColor(176,224,230)},
{"cadet blue", QColor(95,158,160)},
{"steel blue", QColor(70,130,180)},
{"corn flower blue", QColor(100,149,237)},
{"deep sky blue", QColor(0,191,255)},
{"dodger blue", QColor(30,144,255)},
{"light blue", QColor(173,216,230)},
{"sky blue", QColor(135,206,235)},
{"light sky blue", QColor(135,206,250)},
{"midnight blue", QColor(25,25,112)},
{"navy", QColor(0,0,128)},
{"dark blue", QColor(0,0,139)},
{"medium blue", QColor(0,0,205)},
{"blue", QColor(0,0,255)},
{"royal blue", QColor(65,105,225)},
{"blue violet", QColor(138,43,226)},
{"indigo", QColor(75,0,130)},
{"dark slate blue", QColor(72,61,139)},
{"slate blue", QColor(106,90,205)},
{"medium slate blue", QColor(123,104,238)},
{"medium purple", QColor(147,112,219)},
{"dark magenta", QColor(139,0,139)},
{"dark violet", QColor(148,0,211)},
{"dark orchid", QColor(153,50,204)},
{"medium orchid", QColor(186,85,211)},
{"purple", QColor(128,0,128)},
{"thistle", QColor(216,191,216)},
{"plum", QColor(221,160,221)},
{"violet", QColor(238,130,238)},
{"magenta", QColor(255,0,255)},
{"orchid", QColor(218,112,214)},
{"medium violet red", QColor(199,21,133)},
{"pale violet red", QColor(219,112,147)},
{"deep pink", QColor(255,20,147)},
{"hot pink", QColor(255,105,180)},
{"light pink", QColor(255,182,193)},
{"pink", QColor(255,192,203)},
{"antique white", QColor(250,235,215)},
{"beige", QColor(245,245,220)},
{"bisque", QColor(255,228,196)},
{"blanched almond", QColor(255,235,205)},
{"wheat", QColor(245,222,179)},
{"corn silk", QColor(255,248,220)},
{"lemon chiffon", QColor(255,250,205)},
{"light golden rod yellow", QColor(250,250,210)},
{"light yellow", QColor(255,255,224)},
{"saddle brown", QColor(139,69,19)},
{"sienna", QColor(160,82,45)},
{"chocolate", QColor(210,105,30)},
{"peru", QColor(205,133,63)},
{"sandy brown", QColor(244,164,96)},
{"burly wood", QColor(222,184,135)},
{"tan", QColor(210,180,140)},
{"rosy brown", QColor(188,143,143)},
{"moccasin", QColor(255,228,181)},
{"navajo white", QColor(255,222,173)},
{"peach puff", QColor(255,218,185)},
{"misty rose", QColor(255,228,225)},
{"lavender blush", QColor(255,240,245)},
{"linen", QColor(250,240,230)},
{"old lace", QColor(253,245,230)},
{"papaya whip", QColor(255,239,213)},
{"sea shell", QColor(255,245,238)},
{"mint cream", QColor(245,255,250)},
{"slate gray", QColor(112,128,144)},
{"light slate gray", QColor(119,136,153)},
{"light steel blue", QColor(176,196,222)},
{"lavender", QColor(230,230,250)},
{"floral white", QColor(255,250,240)},
{"alice blue", QColor(240,248,255)},
{"ghost white", QColor(248,248,255)},
{"honeydew", QColor(240,255,240)},
{"ivory", QColor(255,255,240)},
{"azure", QColor(240,255,255)},
{"snow", QColor(255,250,250)},
{"black", QColor(0,0,0)},
{"dim gray", QColor(105,105,105)},
{"gray", QColor(128,128,128)},
{"dark gray", QColor(169,169,169)},
{"silver", QColor(192,192,192)},
{"ight gray", QColor(211,211,211)},
{"gainsboro", QColor(220,220,220)},
{"white smoke", QColor(245,245,245)},
{"white", QColor(255,255,255)}};

QVector<QColor> generateColors( const tHighlightingGradient& gradient )
{
    QVector<QColor> result;

    result.push_back(gradient.from);

    int maxIterations = gradient.numberOfColors >= 2 ? gradient.numberOfColors - 2 : 2;

    if( maxIterations > 0 )
    {
        int rStart = gradient.from.red();
        int rEnd = gradient.to.red();
        int rDiff = rEnd - rStart;
        int rStep = rDiff / ( maxIterations + 1 );

        int gStart = gradient.from.green();
        int gEnd = gradient.to.green();
        int gDiff = gEnd - gStart;
        int gStep = gDiff / ( maxIterations + 1 );

        int bStart = gradient.from.blue();
        int bEnd = gradient.to.blue();
        int bDiff = bEnd - bStart;
        int bStep = bDiff / ( maxIterations + 1 );

        for( int i = 1; i <= maxIterations; ++i )
        {
            QColor color;

            color.setRed(rStart + rStep * i);
            color.setGreen(gStart + gStep * i);
            color.setBlue(bStart + bStep * i);

            result.push_back(color);
        }
    }

    result.push_back(gradient.to);

    return result;
}

/// tHighlightingRange
tHighlightingRange::tHighlightingRange( const tHighlightingRangeItem& from_,
                                        const tHighlightingRangeItem& to_,
                                        const QColor& color_,
                                        bool explicitColor_ ):
from(from_), to(to_), color(color_), explicitColor(explicitColor_)
{}

tHighlightingRange::tHighlightingRange():
from(0), to(0), color(0,0,0), explicitColor(false)
{}

bool tHighlightingRange::operator< ( const tHighlightingRange& rVal ) const
{
   bool bResult = false;

   if( from < rVal.from )
   {
       bResult = true;
   }
   else if( from > rVal.from )
   {
       bResult = false;
   }
   else // if from == rVal.from
   {
       if( to < rVal.to )
       {
           bResult = true;
       }
       else
       {
           bResult = false;
       }
   }

   return bResult;
}

/// tRange
tRange::tRange( const tRangeItem& from_, const tRangeItem& to_ ):
from(from_), to(to_)
{}

tRange::tRange():
from(0), to(0)
{}

bool tRange::operator< ( const tRange& rVal ) const
{
    bool bResult = false;

    if( from < rVal.from )
    {
        bResult = true;
    }
    else if( from > rVal.from )
    {
        bResult = false;
    }
    else // if from == rVal.from
    {
        if( to < rVal.to )
        {
            bResult = true;
        }
        else
        {
            bResult = false;
        }
    }

    return bResult;
}

bool tRange::operator== ( const tRange& rVal ) const
{
    return ( from == rVal.from && to == rVal.to );
}


/////////////////////////////tRangePtrWrapper/////////////////////////////
bool tRangePtrWrapper::operator< ( const tRangePtrWrapper& rVal ) const
{
    bool bResult = false;

    if(pRange == nullptr && rVal.pRange != nullptr)
        bResult = true;
    else if(pRange != nullptr && rVal.pRange == nullptr)
        bResult = false;
    else if(pRange == nullptr && rVal.pRange == nullptr)
        bResult = true;

    if( pRange->from < rVal.pRange->from )
    {
        bResult = true;
    }
    else if( pRange->from > rVal.pRange->from )
    {
        bResult = false;
    }
    else // if from == rVal.from
    {
        if( pRange->to < rVal.pRange->to )
        {
            bResult = true;
        }
        else
        {
            bResult = false;
        }
    }

    return bResult;
}

bool tRangePtrWrapper::operator== ( const tRangePtrWrapper& rVal ) const
{
    if(pRange == nullptr && rVal.pRange != nullptr)
        return false;
    else if(pRange != nullptr && rVal.pRange == nullptr)
        return false;
    else if(pRange == nullptr && rVal.pRange == nullptr)
        return true;

    return ( pRange->from == rVal.pRange->from && pRange->to == rVal.pRange->to );
}
//////////////////////////////////////////////////////////////////////////

struct tAnalysisRange
{
    tAnalysisRange(const tRange& inputRange,
                   const tRange& shrinkToRange):
        bFromShrinked(false),
        bToShrinked(false)
    {
        if(inputRange.from < shrinkToRange.from)
        {
            from = shrinkToRange.from;
            bFromShrinked = true;
        }
        else
        {
            from = inputRange.from;
        }

        if(inputRange.to > shrinkToRange.to)
        {
            to = shrinkToRange.to;
            bToShrinked = true;
        }
        else
        {
            to = inputRange.to;
        }
    }

    tRangeItem from;
    bool bFromShrinked;
    tRangeItem to;
    bool bToShrinked;
};

//#define DEBUG_CALC_RANGES
//static const int TRACE_THREASHOLD_NS = 200000;

enum eTreeColumns
{
    eTreeColumn_FoundMatch = 0
};

tTreeItemSharedPtr getMatchesTree( const tFoundMatches& foundMatches )
{
    /////////////////////////////FORM TREE///////////////////////////////////

#ifdef DEBUG_CALC_RANGES
    QElapsedTimer timer;
    timer.start();

    auto nsecStart = timer.nsecsElapsed();
    auto fillInTreeTime = 0u;
#endif

    // we should form a tree of all incoming matches in order to properly assign the colors
    tTreeItemSharedPtr pRootItem =
    std::make_shared<CTreeItem>(nullptr, static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch),
                                CTreeItem::tSortingFunction(),
                                CTreeItem::tHandleDuplicateFunc(),
                                CTreeItem::tFindItemFunc());

    {
        auto formData = [](const tFoundMatch& match) -> CTreeItem::tData
        {
            CTreeItem::tData result;
            result.reserve(1);

            tDataItem matchVariant( &match );
            result.push_back( matchVariant );

            return result;
        };

        auto pCurrentItem = pRootItem.get();

        typedef std::vector<const tFoundMatch*> tStack;
        tStack matchesStack;
        matchesStack.reserve(static_cast<std::size_t>(foundMatches.size()) + 10); // +10 to cover possible overhead of nested groups.

        auto switchToParent = [&pCurrentItem, &matchesStack](int numberOfElementsToPop)
        {
            int counter = 0;

            while(counter < numberOfElementsToPop)
            {
                pCurrentItem = pCurrentItem->getParent();
                ++counter;
            }

            matchesStack.resize(matchesStack.size() - static_cast<std::size_t>(numberOfElementsToPop));
        };

        auto appendChild = [&pCurrentItem, &formData, &matchesStack](const tFoundMatch& match)
        {
            auto data = formData(match);

            assert(false == data.empty());

            tRangePtrWrapper rangePtrWrapper;
            rangePtrWrapper.pRange = &match.range;
            tDataItem rangeVariant( rangePtrWrapper );
            auto* pAddedChild = pCurrentItem->appendChild(rangeVariant, data);
            pCurrentItem = pAddedChild;

            matchesStack.push_back( &match ); // push new element to the stack
        };

        for(const auto& match : foundMatches)
        {
            int counter = 0;

            for(auto it = matchesStack.rbegin(); it != matchesStack.rend(); ++it)
            {
                if( match.range.from >= (*it)->range.from &&
                    match.range.to <= (*it)->range.to ) // if we are still inside the range
                {
                    // break the loop
                    break;
                }
                else // otherwise
                {
                    ++counter; // increment the counter
                }
            }

            if(counter > 0) // if counter is greater than 0
            {
                switchToParent( counter );
            }

#ifdef DEBUG_CALC_RANGES
            auto nsecBeforeFillTree = timer.nsecsElapsed();
#endif
            appendChild(match);

#ifdef DEBUG_CALC_RANGES
            auto nsecAfterFillTree = timer.nsecsElapsed();
            fillInTreeTime += nsecAfterFillTree - nsecBeforeFillTree;
#endif
        }
    }

    /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_CALC_RANGES
    {
        auto nsecBuildTree = timer.nsecsElapsed();
        auto buildTreeTime = nsecBuildTree - nsecStart;
        if(buildTreeTime > TRACE_THREASHOLD_NS)
            SEND_MSG(QString("[%1] fillInTreeTime - %2 nsec; buildTreeTime - %3")
                     .arg(__FUNCTION__)
                     .arg(fillInTreeTime)
                     .arg(buildTreeTime));
    }
#endif

    return pRootItem;
}

tCalcRangesCoverageMulticolorResult calcRangesCoverageMulticolor( const tTreeItemSharedPtr& pMatchesTree,
                               const tRange& inputRange,
                               const tRegexScriptingMetadata& regexScriptingMetadata,
                               const QVector<QColor>& gradientColors,
                               const int& prevColorCounter )
{
#ifdef DEBUG_CALC_RANGES
    QElapsedTimer timer;
    timer.start();

    auto nsecStart = timer.nsecsElapsed();
#endif

    if(true == gradientColors.empty()) // let's reject bad case, when collection of input colors is empty
    {
        tCalcRangesCoverageMulticolorResult dummyResult;
        dummyResult.first = prevColorCounter;
        return dummyResult;
    }

    const auto maxColorsSize = gradientColors.size();

    auto colorsCounter = 0;

    if( prevColorCounter < 0 )
    {
        colorsCounter = 0;
    }
    else if( prevColorCounter >= maxColorsSize )
    {
       colorsCounter = maxColorsSize - 1;
    }
    else
    {
        colorsCounter = prevColorCounter;
    }

    tHighlightingRangeList resultList;
    resultList.reserve(static_cast<std::size_t>(regexScriptingMetadata.getItemsVec().size() + 10)); // +10 to cover overhead which might be caused with nested groups

    tRange rangeToBeAdded; // used only within postVisitFunction
                           // but created outside lambda to minimize reallocations

    auto postVisitFunction = [&resultList, &inputRange, &maxColorsSize, &regexScriptingMetadata, &gradientColors, &colorsCounter, &rangeToBeAdded](tTreeItem* pItem)
    {
        const auto& match = *(pItem->data(static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch)).get<const tFoundMatch*>());

        //SEND_MSG(QString("[calcRangesCoverageMulticolor][postVisitFunction] visit item - |idx:%1;range:from-%2,to-%3;msg-%4|")
        //         .arg(match.idx)
        //         .arg(match.range.from)
        //         .arg(match.range.to)
        //         .arg(*match.pMatchStr));

        if( ( match.range.from < inputRange.from && match.range.to < inputRange.from ) )
        {
            // we are outside target range.
            // We can skip further analysis of this match.
            return true;
        }
        else if( match.range.from > inputRange.to && match.range.to > inputRange.to )
        {
            // we are outside target range and matches are beyond the range.
            // We can skip further iteration over the tree.
            return false;
        }

        tAnalysisRange analysisRange( match.range, inputRange );
        const auto& matchIdx = match.idx;
        bool bToShrinked = analysisRange.bToShrinked;

        auto getColor = [&regexScriptingMetadata,
                &gradientColors,
                &colorsCounter,
                &maxColorsSize,
                &matchIdx,
                &bToShrinked]()->QPair<bool /*isExplicit*/, QColor>
        {

            QOptionalColor scriptedColor;
            scriptedColor.isSet = false;

            if( matchIdx < regexScriptingMetadata.getItemsVec().size() )
            {
                scriptedColor = regexScriptingMetadata.getItemsVec().operator[](matchIdx)->highlightingColor;
            }

            QColor color;
            bool bIsExplicitColor = false;

            if(true == scriptedColor.isSet)
            {
                color = scriptedColor.color;
                bIsExplicitColor = true;
            }
            else
            {
                color = gradientColors[colorsCounter];
            }

            if( false == bToShrinked  )
            {
                colorsCounter = ++colorsCounter % maxColorsSize;
            }

            return QPair<bool /*isExplicit*/, QColor>(bIsExplicitColor, color);
        };

        // we need to analyze first level children of pItem and split the ranges
        const auto& children = pItem->getChildren();

        auto selectedColor = getColor();

        if(false == children.isEmpty()) // if there are some children available
        {
            // we need to process "from beginning to first child" case manually
            {
                const auto& firstChild = *children.begin();
                const auto& firstChildMatch = *(firstChild->data(static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch)).get<const tFoundMatch*>());
                tAnalysisRange firstChildAnalysisRange(firstChildMatch.range, inputRange);

                if( firstChildAnalysisRange.from > analysisRange.from ) // if there is room for additional range
                {
                    // let's add it
                    rangeToBeAdded.from = analysisRange.from;
                    rangeToBeAdded.to = firstChildAnalysisRange.from - 1;

                    resultList.push_back( tHighlightingRange( rangeToBeAdded.from,
                                                              rangeToBeAdded.to,
                                                              selectedColor.second,
                                                              selectedColor.first ) );
                }
            }

            // wee need to process "between children" cases in a loop ( in case if there are 2 children or more )
            if(children.size() >= 2)
            {
                for(auto it = children.begin(); it != children.end() - 1; ++it)
                {
                    const auto& firstChildMatch = *((*it)->data(static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch)).get<const tFoundMatch*>());
                    tAnalysisRange firstChildAnalysisRange( firstChildMatch.range, inputRange );

                    const auto& secondChildMatch = *((*(it+1))->data(static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch)).get<const tFoundMatch*>());
                    tAnalysisRange secondChildAnalysisRange( secondChildMatch.range, inputRange );

                    if((secondChildAnalysisRange.from - firstChildAnalysisRange.to) > 1)
                    {
                        rangeToBeAdded.from = firstChildAnalysisRange.to+1;
                        rangeToBeAdded.to = secondChildAnalysisRange.from-1;

                        resultList.push_back( tHighlightingRange( rangeToBeAdded.from,
                                                                  rangeToBeAdded.to,
                                                                  selectedColor.second,
                                                                  selectedColor.first ) );
                    }
                }
            }

            // we need to process "from last child to end" case manually
            {
                const auto& lastChild = *(children.end() - 1);
                const auto& lastChildMatch = *(lastChild->data(static_cast<int>(eTreeColumns::eTreeColumn_FoundMatch)).get<const tFoundMatch*>());
                tAnalysisRange lastChildAnalysisRange(lastChildMatch.range, inputRange);

                if( analysisRange.to > lastChildAnalysisRange.to ) // if there is room for additional range
                {
                    // let's add it
                    rangeToBeAdded.from = lastChildAnalysisRange.to + 1;
                    rangeToBeAdded.to = analysisRange.to;

                    resultList.push_back( tHighlightingRange( rangeToBeAdded.from,
                                                              rangeToBeAdded.to,
                                                              selectedColor.second,
                                                              selectedColor.first ) );
                }
            }
        }
        else // if there are no children
        {
            // let's add item unconditionally
            resultList.push_back( tHighlightingRange( analysisRange.from,
                                                      analysisRange.to,
                                                      selectedColor.second,
                                                      selectedColor.first ) );
        }

        return true;
    };

    pMatchesTree->visit(CTreeItem::tVisitFunction(), postVisitFunction, false, true, false);

    int normalizationIdx = inputRange.from;

    if(0 != normalizationIdx)
    {
        for(auto& resultListItem : resultList)
        {
            resultListItem.from = resultListItem.from - normalizationIdx;
            resultListItem.to = resultListItem.to - normalizationIdx;
        }
    }

    tCalcRangesCoverageMulticolorResult result;

    result.first = colorsCounter;

    result.second.insert(resultList.begin(), resultList.end());

    //for( const auto& resultItem : resultList )
    //{
        //SEND_MSG(QString("[calcRangesCoverageMulticolor][result] - |from-%1;to-%2;red-%3;green-%4;blue-%5,expColor-%6|")
        //        .arg(resultItem.from)
        //        .arg(resultItem.to)
        //        .arg(resultItem.color.red())
        //        .arg(resultItem.color.green())
        //        .arg(resultItem.color.blue())
        //        .arg(resultItem.explicitColor));
    //}

#ifdef DEBUG_CALC_RANGES
    auto nsecEnd = timer.nsecsElapsed();
    auto timeVal = nsecEnd - nsecStart;
    if(timeVal > TRACE_THREASHOLD_NS)
        SEND_MSG(QString("[%1] Spent time - %2 nsec")
                 .arg(__FUNCTION__)
                 .arg(nsecEnd - nsecStart));
#endif

    return result;
}

//tItemMetadata
tItemMetadata::tItemMetadata(): msgId(-1),
    highlightingInfoMultiColor(),
    fieldRanges(),
    strSize(0),
    msgSize(0u),
    timeStamp(0u)
{

}

tItemMetadata::tItemMetadata( const tMsgId& msgId_,
                              const tMsgId& msgIdFiltered_,
                              const tFieldRanges& fieldRanges_,
                              const int& strSize_,
                              const unsigned int& msgSize_,
                              const unsigned int& timeStamp_):
    msgId(msgId_),
    msgIdFiltered(msgIdFiltered_),
    fieldRanges(fieldRanges_),
    strSize(strSize_),
    msgSize(msgSize_),
    timeStamp(timeStamp_)
{

}

void tItemMetadata::updateHighlightingInfo( const tFoundMatches& foundMatches, const QVector<QColor>& gradientColors, const tRegexScriptingMetadata& regexScriptingMetadata )
{
    QElapsedTimer timer;
    timer.start();

    auto colorsCounter = 0;

    auto pMatchesTree = getMatchesTree(foundMatches);

    for( auto it = fieldRanges.begin(); it != fieldRanges.end(); ++it )
    {
        auto highlightingRangesMulticolorRes = calcRangesCoverageMulticolor( pMatchesTree, it.value(), regexScriptingMetadata, gradientColors, colorsCounter );

        colorsCounter = highlightingRangesMulticolorRes.first;

        if(false == highlightingRangesMulticolorRes.second.empty())
        {
            highlightingInfoMultiColor.insert( it.key(), highlightingRangesMulticolorRes.second );
        }
    }

    //static std::atomic<int> counter(0);

    //if(counter % 1000 == 0)
    //{
    //    auto elapsedTimeNano = timer.nsecsElapsed();
    //    SEND_MSG(QString("[%1] Item #%2 - elapsed time - %3").arg(__FUNCTION__).arg(counter).arg(elapsedTimeNano));
    //}

    //++counter;
}

//tFoundMatch
tFoundMatch::tFoundMatch():
pMatchStr(std::make_shared<QString>()),
range(0,0),
idx(0),
msgSizeBytes(0u),
timeStamp(0u),
msgId(0)
{}

tFoundMatch::tFoundMatch( const tQStringPtr& pMatchStr_,
                          const tRange& range_,
                          const int& idx_,
                          const unsigned int& msgSizeBytes_,
                          const unsigned int& timeStamp_,
                          const tMsgId& msgId_):
pMatchStr((nullptr!=pMatchStr_)?pMatchStr_:std::make_shared<QString>()),
range(range_),
idx(idx_),
msgSizeBytes(msgSizeBytes_),
timeStamp(timeStamp_),
msgId(msgId_)
{}

bool tFoundMatch::operator< (const tFoundMatch& rhs) const
{
    return msgId < rhs.msgId;
}

//tFoundMatchesPackItem
tFoundMatchesPackItem::tFoundMatchesPackItem()
{

}

tFoundMatchesPackItem::tFoundMatchesPackItem( tItemMetadata&& itemMetadata_,
                                              tFoundMatches&& foundMatches_ ):
  mItemMetadata(itemMetadata_),
  mFoundMatches(foundMatches_)
{
}

const tItemMetadata& tFoundMatchesPackItem::getItemMetadata() const
{
    return mItemMetadata;
}

const tFoundMatches& tFoundMatchesPackItem::getFoundMatches() const
{
    return mFoundMatches;
}

//tFoundMatchesPack
tFoundMatchesPack::tFoundMatchesPack():
matchedItemVec()
{

}

tFoundMatchesPack::tFoundMatchesPack( const tFoundMatchesPackItemVec& matchedItemVec_ ):
matchedItemVec(matchedItemVec_)
{

}

QString getName(eSearchResultColumn val)
{
    QString result;

    switch(val)
    {
        case eSearchResultColumn::Apid:
        {
            result = "Apid";
        }
            break;
        case eSearchResultColumn::Args:
        {
            result = "Args";
        }
            break;
        case eSearchResultColumn::Ctid:
        {
            result = "Ctid";
        }
            break;
        case eSearchResultColumn::Last:
        {
            result = "Last";
        }
            break;
        case eSearchResultColumn::Mode:
        {
            result = "Mode";
        }
            break;
        case eSearchResultColumn::Time:
        {
            result = "Time";
        }
            break;
        case eSearchResultColumn::Type:
        {
            result = "Type";
        }
            break;
        case eSearchResultColumn::Count:
        {
            result = "Count";
        }
            break;
        case eSearchResultColumn::Ecuid:
        {
            result = "Ecuid";
        }
            break;
        case eSearchResultColumn::Index:
        {
            result = "Index";
        }
            break;
        case eSearchResultColumn::Subtype:
        {
            result = "Subtype";
        }
            break;
        case eSearchResultColumn::SessionId:
        {
            result = "SessionId";
        }
            break;
        case eSearchResultColumn::Timestamp:
        {
            result = "Timestamp";
        }
        break;
        case eSearchResultColumn::Payload:
        {
            result = "Payload";
        }
        break;
    }

    return result;
}

QString getName(ePatternsColumn val)
{
    QString result;

    switch(val)
    {
        case ePatternsColumn::Alias:
        {
            result = "Full alias";
        }
            break;
        case ePatternsColumn::AliasTreeLevel:
        {
            result = "Alias";
        }
        break;
        case ePatternsColumn::Default:
        {
            result = "Def.";
        }
            break;
        case ePatternsColumn::Combine:
        {
            result = "Comb.";
        }
            break;
        case ePatternsColumn::Regex:
        {
            result = "Regex";
        }
            break;
        case ePatternsColumn::AfterLastVisible:
        {
            result = "After last visible";
        }
            break;
        case ePatternsColumn::RowType:
        {
            result = "Row type";
        }
            break;
        case ePatternsColumn::IsFiltered:
        {
            result = "Is filtered";
        }
            break;
        case ePatternsColumn::Last:
        {
            result = "Last";
        }
            break;
    }

    return result;
}

ePatternsColumn toPatternsColumn(int column)
{
    return static_cast<ePatternsColumn>(column);
}

//tGroupedViewMetadata
tGroupedViewMetadata::tGroupedViewMetadata():
timeStamp(0u),
msgId(0)
{
}

bool tGroupedViewMetadata::operator< (const tGroupedViewMetadata& rhs) const
{
    return msgId < rhs.msgId;
}

bool tGroupedViewMetadata::operator== (const tGroupedViewMetadata& rhs) const
{
    return msgId == rhs.msgId;
}

tGroupedViewMetadata::tGroupedViewMetadata( const unsigned int timeStamp_, const tMsgId& msgId_ ):
timeStamp(timeStamp_),
msgId(msgId_)
{
}

QString getName(eGroupedViewColumn val)
{
    QString result;

    switch(val)
    {
        case eGroupedViewColumn::SubString:
        {
            result = "Found sub-string";
        }
            break;
        case eGroupedViewColumn::Messages:
        {
            result = "Msg-s";
        }
            break;
        case eGroupedViewColumn::MessagesPercantage:
        {
            result = "Msg-s, %";
        }
            break;
        case eGroupedViewColumn::MessagesPerSecondAverage:
        {
            result = "Msg-s/sec, av.";
        }
            break;
        case eGroupedViewColumn::Payload:
        {
            result = "Payload";
        }
            break;
        case eGroupedViewColumn::PayloadPercantage:
        {
            result = "Payload, %";
        }
            break;
        case eGroupedViewColumn::PayloadPerSecondAverage:
        {
            result = "Payload, b/sec, av.";
        }
            break;
        case eGroupedViewColumn::AfterLastVisible:
        {
            result = "Last";
        }
            break;
        case eGroupedViewColumn::Metadata:
        {
            result = "Metadata";
        }
            break;
        case eGroupedViewColumn::Last:
        {
            result = "Last";
        }
            break;
    }

    return result;
}

eGroupedViewColumn toGroupedViewColumn(int column)
{
    return static_cast<eGroupedViewColumn>(column);
}

QString getName(eRegexFiltersColumn val)
{
    QString result;

    switch(val)
    {
        case eRegexFiltersColumn::Index:
        {
            result = "Index";
        }
        break;
        case eRegexFiltersColumn::ItemType:
        {
            result = "Item type";
        }
            break;
        case eRegexFiltersColumn::Value:
        {
            result = "Value";
        }
            break;
        case eRegexFiltersColumn::AfterLastVisible:
        {
            result = "Last";
        }
            break;
        case eRegexFiltersColumn::Color:
        {
            result = "Color";
        }
            break;
        case eRegexFiltersColumn::Range:
        {
            result = "Range";
        }
            break;
        case eRegexFiltersColumn::RowType:
        {
            result = "Row type";
        }
            break;
        case eRegexFiltersColumn::IsFiltered:
        {
            result = "Is filtered";
        }
            break;
        case eRegexFiltersColumn::GroupName:
        {
            result = "Group name";
        }
            break;
        case eRegexFiltersColumn::GroupSyntaxType:
        {
            result = "Group Syntax type";
        }
            break;
        case eRegexFiltersColumn::Last:
        {
            result = "Last";
        }
            break;
    }

    return result;
}

eRegexFiltersColumn toRegexFiltersColumn(int column)
{
    return static_cast<eRegexFiltersColumn>(column);
}

QString getName(eRegexFiltersRowType val)
{
    QString result;

    switch(val)
    {
        case eRegexFiltersRowType::Text:
        {
            result = "Text";
        }
        break;
        case eRegexFiltersRowType::VarGroup:
        {
            result = "Variable";
        }
            break;
        case eRegexFiltersRowType::NonVarGroup:
        {
            result = "Group";
        }
            break;
        case eRegexFiltersRowType::NonCapturingGroup:
        {
            result = "Non-capturing group";
        }
            break;
    }

    return result;
}

QString getName(eRequestState field)
{
    QString result;

    switch(field)
    {
        case eRequestState::ERROR_STATE:
        {
            result = "Error";
        }
            break;
        case eRequestState::PROGRESS:
        {
            result = "Progress";
        }
            break;
        case eRequestState::SUCCESSFUL:
        {
            result = "Successful";
        }
            break;
    }

    return result;
}

tCacheSizeB MBToB( const tCacheSizeMB& mb )
{
    return static_cast<tCacheSizeB>(mb) * 1024u * 1024u;
}

tCacheSizeMB BToMB( const tCacheSizeB& b )
{
    return static_cast<tCacheSizeMB>(b / 1024u / 1024u);
}

// tHighlightingGradient
tHighlightingGradient::tHighlightingGradient():
from(), to(), numberOfColors(0)
{

}

tHighlightingGradient::tHighlightingGradient(const QColor& from_, const QColor& to_, int numberOfColors_):
from(from_), to(to_), numberOfColors(numberOfColors_)
{

}

bool tHighlightingGradient::operator==(const tHighlightingGradient& rhs) const
{
    return (from == rhs.from && to == rhs.to && numberOfColors == rhs.numberOfColors);
}

bool tHighlightingGradient::operator!=(const tHighlightingGradient& rhs) const
{
    return !( *this == rhs );
}

tRegexScriptingMetadataItemPtr parseRegexGroupName( const QString& groupName )
{
    QStringList splitGroupName = groupName.split(sRegexScriptingDelimiter,
                                                 QString::SplitBehavior::SkipEmptyParts,
                                                 Qt::CaseInsensitive);

    tRegexScriptingMetadataItem result;

    // static const section
    static const QString rgbRegexStr( QString("%1([0-9]{1,3})_([0-9]{1,3})_([0-9]{1,3})").arg(sRGBPrefix) );
    static const QRegularExpression rgbRegex(rgbRegexStr, QRegularExpression::CaseInsensitiveOption);
    static const QString varRegexStr( QString("%1([\\w\\d]+)").arg(sVARPrefix) );
    static const QRegularExpression varRegex(varRegexStr, QRegularExpression::CaseInsensitiveOption);

    auto normalizeRGBItem = [](const int& rgbItem)->int
    {
        int normalizationResult = 0;

        if(rgbItem > 255)
        {
            normalizationResult = 255;
        }
        else if( rgbItem < 0 )
        {
            normalizationResult = 0;
        }
        else
        {
            normalizationResult = rgbItem;
        }

        return normalizationResult;
    };

    QOptionalColor optColor;
    optColor.isSet = false;

    tOptionalVarName optVarName;
    optVarName.first = false;

    for( const auto& groupNamePart : splitGroupName )
    {
        // parse color
        if(false == optColor.isSet)
        {
            QRegularExpressionMatch rgbMatch = rgbRegex.match(groupNamePart);

            if(rgbMatch.hasMatch())
            {
                bool scriptedColorFound = rgbMatch.lastCapturedIndex() == 3                    // if all 3 groups found
                                       && rgbMatch.capturedEnd(3) == groupNamePart.length();   // and if last group's ending index is equal to the group name's length

                if(scriptedColorFound)
                {
                    bool bRedParsed = false;
                    int red = 0;
                    bool bGreenParsed = false;
                    int green = 0;
                    bool bBlueParsed = false;
                    int blue = 0;

                    for (int i = 0; i <= rgbMatch.lastCapturedIndex(); ++i) // let's iterate over the groups
                    {
                        const auto& matchItem = rgbMatch.captured(i);

                        if(i>0) // 0 index stands for the whole matched string
                        {
                            if(0 != matchItem.size()) // if match is non-zero string
                            {
                                switch (i)
                                {
                                    case 1: // R
                                    {
                                        red = matchItem.toInt( &bRedParsed, 10 );

                                        if(true == bRedParsed)
                                        {
                                            red = normalizeRGBItem(red);
                                        }
                                    }
                                        break;
                                    case 2: // G
                                    {
                                        green = matchItem.toInt( &bGreenParsed, 10 );

                                        if(true == bGreenParsed)
                                        {
                                            green = normalizeRGBItem(green);
                                        }
                                    }
                                        break;
                                    case 3: // B
                                    {
                                        blue = matchItem.toInt( &bBlueParsed, 10 );

                                        if(true == bBlueParsed)
                                        {
                                            blue = normalizeRGBItem(blue);
                                        }
                                    }
                                        break;

                                    default:
                                        break;
                                }
                            }
                        }
                    }

                    if( true == bRedParsed
                     && true == bGreenParsed
                     && true == bBlueParsed)
                    {
                        optColor.isSet = true;
                        optColor.color = QColor(red, green, blue);
                    }
                }
            }
            else
            {
                QString lowerGroupNamePart = groupNamePart.toLower();

                auto foundColor = sColorsMap.find(lowerGroupNamePart);

                if(sColorsMap.end() != foundColor)
                {
                    optColor.isSet = true;
                    optColor.color = foundColor->second;
                }
            }
        }

        // parse var
        if(false == optVarName.first)
        {
            QRegularExpressionMatch varMatch = varRegex.match(groupNamePart);

            if(varMatch.hasMatch())
            {
                bool scriptedVarFound = varMatch.lastCapturedIndex() == 1;           // if 1 group found

                if(true == scriptedVarFound)
                {
                    optVarName.first = true;
                    optVarName.second = varMatch.captured(1);
                }
            }
        }

        if(true == optVarName.first
        && true == optColor.isSet) // if we've found everything, lets break the loop
        {
            break;
        }
    }

    tRegexScriptingMetadataItemPtr pItem = std::make_shared<tRegexScriptingMetadataItem>();
    pItem->highlightingColor = optColor;
    pItem->varName = optVarName;

    return pItem;
}

//tRegexScriptingMetadata
bool tRegexScriptingMetadata::parse(const QRegularExpression& regex)
{
    bool bResult = true;

    if(true == regex.isValid())
    {
        auto groupNames = regex.namedCaptureGroups();

        for(const auto& groupName : groupNames)
        {
            mItemsVec.push_back(parseRegexGroupName(groupName));
        }
    }
    else
    {
        bResult = false;
    }

    return bResult;
}

const tRegexScriptingMetadataItemPtrVec& tRegexScriptingMetadata::getItemsVec() const
{
    return mItemsVec;
}

//////////////////////////////////////////////////////////

Qt::CheckState V_2_CS( const tDataItem& val )
{
    return val.get<Qt::CheckState>();
}

Qt::CheckState V_2_CS( const QVariant& val )
{
    return static_cast<Qt::CheckState>( val.value<int>() );
}

QVariant toQVariant(const tDataItem& item)
{
    QVariant result;

    // holds QString, Qt::CheckState, ePatternsRowType, bool, int, tGroupedViewMetadata

    if(item.index() == tDataItem::index_of<QString>())
    {
        result.setValue(item.get<QString>());
    }
    else if(item.index() == tDataItem::index_of<Qt::CheckState>())
    {
        result.setValue(item.get<Qt::CheckState>());
    }
    else if(item.index() == tDataItem::index_of<ePatternsRowType>())
    {
        result.setValue(item.get<ePatternsRowType>());
    }
    else if(item.index() == tDataItem::index_of<bool>())
    {
        result.setValue(item.get<bool>());
    }
    else if(item.index() == tDataItem::index_of<int>())
    {
        result.setValue(item.get<int>());
    }
    else if(item.index() == tDataItem::index_of<double>())
    {
        result.setValue(item.get<double>());
    }
    else if(item.index() == tDataItem::index_of<tGroupedViewMetadata>())
    {
        result.setValue(item.get<tGroupedViewMetadata>());
    }
    else if(item.index() == tDataItem::index_of<tRangePtrWrapper>())
    {
        result.setValue(item.get<tRangePtrWrapper>());
    }
    else if(item.index() == tDataItem::index_of<tFoundMatch*>())
    {
        result.setValue(item.get<const tFoundMatch*>());
    }
    else if(item.index() == tDataItem::index_of<tColorWrapper>())
    {
        result.setValue(item.get<tColorWrapper>());
    }
    else if(item.index() == tDataItem::index_of<tRange>())
    {
        result.setValue(item.get<tRange>());
    }
    else if(item.index() == tDataItem::index_of<eRegexFiltersRowType>())
    {
        result.setValue(item.get<eRegexFiltersRowType>());
    }

    return result;
}

tDataItem toRegexDataItem(const QVariant& variant, const eRegexFiltersColumn& column)
{
    tDataItem result;

    switch(column)
    {
        case eRegexFiltersColumn::Index:
        {
            result = variant.value<int>();
        }
        break;
        case eRegexFiltersColumn::ItemType:
        case eRegexFiltersColumn::Value:
        {
            result = variant.value<QString>();
        }
            break;
        case eRegexFiltersColumn::AfterLastVisible:
        {
            result = QString();
        }
            break;
        case eRegexFiltersColumn::Color:
        {
            result = variant.value<tColorWrapper>();
        }
            break;
        case eRegexFiltersColumn::Range:
        {
            result = variant.value<tRange>();
        }
            break;
        case eRegexFiltersColumn::RowType:
        {
            result = variant.value<eRegexFiltersRowType>();
        }
            break;
        case eRegexFiltersColumn::IsFiltered:
        {
            result = variant.value<bool>();
        }
            break;
        case eRegexFiltersColumn::GroupName:
        {
            result = variant.value<QString>();
        }
            break;
        case eRegexFiltersColumn::Last:
        {
            result = QString();
        }
            break;
    }

    return result;
}

bool QOptionalColor::operator== ( const QOptionalColor& rhs ) const
{
    return color == rhs.color && isSet == rhs.isSet;
}

bool tColorWrapper::operator< ( const tColorWrapper& rhs ) const
{
    bool bResult;

    if(optColor.isSet < rhs.optColor.isSet)
    {
        bResult = true;
    }
    else
    {
        if( optColor.color.red() < rhs.optColor.color.red() )
        {
            bResult = true;
        }
        else if( optColor.color.red() > rhs.optColor.color.red() )
        {
            bResult = false;
        }
        else
        {
            if( optColor.color.green() < rhs.optColor.color.green() )
            {
                bResult = true;
            }
            else if( optColor.color.green() > rhs.optColor.color.green() )
            {
                bResult = false;
            }
            else
            {
                if( optColor.color.blue() < rhs.optColor.color.blue() )
                {
                    bResult = true;
                }
                else
                {
                    bResult = false;
                }
            }
        }
    }

    return bResult;
}

bool tColorWrapper::operator== ( const tColorWrapper& rhs ) const
{
    return optColor == rhs.optColor;
}
