#-- Load Network Visualization Dataframes --#
bhnodes <- read.csv("bh_nodes_temp.csv")
bhlinks <- read.csv("bh_links_temp.csv")

# raw
forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
             Target = "target", Value = "value", NodeID = "name",
             Nodesize = "degree", Group = "group",
             opacity = .9, zoom=TRUE,
             linkColour = ifelse(bhlinks$value > 0, "585C5C","F3F3F3"),
             fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
             legend = TRUE)

# raw
forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
             Target = "target", Value = "value", NodeID = "name",
             Nodesize = "degree", Group = "greedy_priority",
             opacity = .9, zoom=TRUE,
             linkColour = ifelse(bhlinks$value > 0, "585C5C","F3F3F3"),
             fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
             legend = TRUE)

# raw
forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
             Target = "target", Value = "value", NodeID = "name",
             Nodesize = "degree", Group = "greedy_priority",
             opacity = .9, zoom=TRUE,
             linkColour = ifelse(bhlinks$greedy_pairing > 0, "585C5C","F3F3F3"),
             fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
             legend = TRUE)
