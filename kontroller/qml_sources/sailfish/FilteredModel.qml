import QtQuick 2.0

Item {
// not yet supported
//    enum FilterType
//    {
//        StartsWith,
//        Contains
//    }
// so stick with int

    property int type: 0

    property var sourceModel: null

    property var filteredModel: null

    property string filter: value

    /**
     * \brief the properties on which the filter is tested
     */
    property var properties: []

    onSourceModelChanged: {
        updateFilteredModel()
    }

    onTypeChanged: {
        updateFilteredModel()
    }

    onFilterChanged: {
        updateFilteredModel()
    }

    function updateFilteredModel()
    {
        if(sourceModel !== null)
        {
            if(filter === null || filter === "")
            {
                filteredModel = sourceModel
                return;
            }
            var filtered = [];
            for(var item in sourceModel)
            {
                if(isAMatch(sourceModel[item]))
                {
                    filtered.push(sourceModel[item]);
                }
            }
            filteredModel = filtered;
        }
        else
            filteredModel = [];
    }

    function isAMatch(item)
    {
        if(Array.isArray(properties))
        {
            for(var i = 0; i < properties.length; ++i)
            {
                var test = item[properties[i]];
                if(test instanceof String || typeof(test) === "string")
                {
                    if(isPropertyMatch(test))
                        return true;
                }
            }
        }
        else if(properties instanceof string || typeof(properties) === "string")
        {
            return isPropertyMatch(item[properties]);
        }
        return false;
    }

    function isPropertyMatch(test)
    {
        var value = test.toLowerCase().indexOf(filter.toLowerCase());
        if(type === 0) //FilterType.StartsWith
        {
            return value === 0;
        }
        else if(type === 1) // FilterType.Contains
        {
            return value >= 0;
        }
        return false;
    }
}
