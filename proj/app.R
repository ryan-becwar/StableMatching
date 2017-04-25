library(ggplot2)
library(plotly)
library(networkD3)
require(data.table)

# -------------------------------------------------------------------
# USER INTERFACE
# -------------------------------------------------------------------
ui <- fluidPage(
  titlePanel("Data Science Project 2017"),

  sidebarLayout(
    sidebarPanel(
      sliderInput("opacity", "Opacity", 0.8, min = 0.1,
                    max = 1, step = .1),

      selectInput("dataset", label = h4("Choose a dataset."),
                   choices = list("BH1987", "KB2009")),

      radioButtons("stage", label = h4("Choose a stage in the matching process."),
                   choices = list("Raw Network", "Network with Priority",
                                  "Network with Final Pairings"), selected = "Raw Network")
    ),
    mainPanel(
      tabsetPanel(
        tabPanel("Greedy Algorithim", forceNetworkOutput("greedy")),
        tabPanel("Pagerank Algorithim", forceNetworkOutput("pagerank")),
        tabPanel("Regret Algorithim", forceNetworkOutput("regret"))
      )
    )
  ),

  fluidRow(
    column(width = 12, class = "well",
      h4("Plotly Example"),
      plotlyOutput("plot1", height = 700)
      )
   ),

  fluidRow(
    column(width = 4, class = "well",
           h4("Here are some buttons"),
           checkboxGroupInput("datasetGroup",
                              label = h3("Pick datasets to view."),
                              choices = list("Dataset1" = 1,
                                             "Dataset2" = 2, "Dataset3" = 3),
                              selected = 1)
    ),
    column(width = 4, class = "well",
           h4("We can compare this graph")
    ),
    column(width = 4, class = "well",
           h4("with this graph")
    )
  )
)

# -------------------------------------------------------------------
# Standard deviation function
# -------------------------------------------------------------------
sdevTop <-function(inputList) {
inputAvg <- mean(inputList)
inputsd <- sd(inputList)
Output <- inputAvg + inputsd
return(Output)
}

sdevBot <-function(inputList) {
inputAvg <- mean(inputList)
inputsd <- sd(inputList)
Output <- inputAvg - inputsd
return(Output)
}


# -------------------------------------------------------------------
# SERVER PORTION
# -------------------------------------------------------------------
server <- function(input, output) {

  #-- Load Network Visualization Dataframes --#
  bhnodes <- read.csv("bh_nodes_temp.csv")
  bhlinks <- read.csv("bh_links_temp.csv")

  kbnodes <- read.csv("kb_nodes.csv")
  kblinks <- read.csv("kb_links.csv")

  all_nodes <- list(kbnodes, bhnodes)
  all_links <- list(kblinks, bhlinks)

  #-- Get Dataset for Network from User Input --#
  get_nodes <- reactive({switch(input$dataset,"KB2009" = kbnodes,"BH1987" = bhnodes)})
  get_links <- reactive({switch(input$dataset,"KB2009" = kblinks,"BH1987" = bhlinks)})
  

  greedy_group <- reactive({switch(input$stage, "Raw Network" = "group",
                                      "Network with Priority" = "greedy_priority", 
                                      "Network with Final Pairings" = "greedy_priority")})
  
  pagerank_group <- reactive({switch(input$stage, "Raw Network" = "group",
                                      "Network with Priority" = "pagerank_priority", 
                                      "Network with Final Pairings" = "pagerank_priority")})
  
  regret_group <- reactive({switch(input$stage, "Raw Network" = "group",
                                      "Network with Priority" = "regret_priority", 
                                      "Network with Final Pairings" = "regret_priority")})
  
  greedy_pairing <- reactive({switch(input$stage, "Raw Network" = bhlinks$value,
                                   "Network with Priority" = bhlinks$value, 
                                   "Network with Final Pairings" = bhlinks$greedy_pairing)})
  
  pagerank_pairing <- reactive({switch(input$stage, "Raw Network" = bhlinks$value,
                                     "Network with Priority" = bhlinks$value, 
                                     "Network with Final Pairings" = bhlinks$pagerank_pairing)})
  
  regret_pairing <- reactive({switch(input$stage, "Raw Network" = bhlinks$value,
                                     "Network with Priority" = bhlinks$value, 
                                     "Network with Final Pairings" = bhlinks$regret_pairing)})
  
  output$greedy <- renderForceNetwork({
    forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = greedy_group(), 
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(greedy_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })

  output$pagerank <- renderForceNetwork({
    forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = pagerank_group(), 
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(pagerank_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })
  
  output$regret <- renderForceNetwork({
    forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = regret_group(), 
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(regret_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })


  ##### GRAPH STUFF #####
  ranges <- reactiveValues(x = NULL, y = NULL)

  data = read.csv("value_data.csv")
  df = data.frame(noise= data$noise,
                  greedy=data$greedy_mean,
                  pagerank=data$pagerank_value,
                  optimal=data$optimal_value)

  # -------------------------------------------------------------------
  # Linked plots (middle and right)
  ranges2 <- reactiveValues(x = NULL, y = NULL)


  #-- Line Graph Visualizations --#
  data = read.csv("../results/small_incr_results.csv")
  df = data.frame(noise= data$noise,
                  greedy=data$greedy_mean,
                  pagerank=data$pagerank_value,
                  regret=data$regret_value,
                  optimal=data$optimal_value)
  dfReduced = data.frame(noise= data$noise,
             greedy= data$greedy_mean)
  dt <- data.table(df)

  #At each noise value, take only the max, min and mean value
  greedyMax = dt[ , sdevTop(greedy), by = noise]
  greedyMin = dt[ , sdevBot(greedy), by = noise]
  greedyAvg = dt[ , mean(greedy), by = noise]

  pagerankMax = dt[ , sdevTop(pagerank), by = noise]
  pagerankMin = dt[ , sdevBot(pagerank), by = noise]
  pagerankAvg = dt[ , mean(pagerank), by = noise]

  regretMax = dt[ , sdevTop(regret), by = noise]
  regretMin = dt[ , sdevBot(regret), by = noise]
  regretAvg = dt[ , mean(regret), by = noise]

  optimalMax = dt[ , sdevTop(optimal), by = noise]
  optimalMin = dt[ , sdevBot(optimal), by = noise]
  optimalAvg = dt[ , mean(optimal), by = noise]


  #The above functions rename greedy to V1, so they must be renamed
  dfGreedyMax <- as.data.frame(greedyMax)
  colnames(dfGreedyMax)[colnames(dfGreedyMax)=="V1"] <- "Max"

  dfGreedyMin <- as.data.frame(greedyMin)
  colnames(dfGreedyMin)[colnames(dfGreedyMin)=="V1"] <- "Min"

  dfGreedyAvg <- as.data.frame(greedyAvg)
  colnames(dfGreedyAvg)[colnames(dfGreedyAvg)=="V1"] <- "Avg"

  dfGreedyComplete <- merge(merge(dfGreedyMax, dfGreedyMin, by = "noise"), dfGreedyAvg, by = "noise")

  #The above functions rename pagerank to V1, so they must be renamed
  dfPagerankMax <- as.data.frame(pagerankMax)
  colnames(dfPagerankMax)[colnames(dfPagerankMax)=="V1"] <- "Max"

  dfPagerankMin <- as.data.frame(pagerankMin)
  colnames(dfPagerankMin)[colnames(dfPagerankMin)=="V1"] <- "Min"

  dfPagerankAvg <- as.data.frame(pagerankAvg)
  colnames(dfPagerankAvg)[colnames(dfPagerankAvg)=="V1"] <- "Avg"

  dfPagerankComplete <- merge(merge(dfPagerankMax, dfPagerankMin, by = "noise"), dfPagerankAvg, by = "noise")

  #The above functions rename regret to V1, so they must be renamed
  dfRegretMax <- as.data.frame(regretMax)
  colnames(dfRegretMax)[colnames(dfRegretMax)=="V1"] <- "Max"

  dfRegretMin <- as.data.frame(regretMin)
  colnames(dfRegretMin)[colnames(dfRegretMin)=="V1"] <- "Min"

  dfRegretAvg <- as.data.frame(regretAvg)
  colnames(dfRegretAvg)[colnames(dfRegretAvg)=="V1"] <- "Avg"

  dfRegretComplete <- merge(merge(dfRegretMax, dfRegretMin, by = "noise"), dfRegretAvg, by = "noise")

  #The above functions rename optimal to V1, so they must be renamed
  dfOptimalMax <- as.data.frame(optimalMax)
  colnames(dfOptimalMax)[colnames(dfOptimalMax)=="V1"] <- "Max"

  dfOptimalMin <- as.data.frame(optimalMin)
  colnames(dfOptimalMin)[colnames(dfOptimalMin)=="V1"] <- "Min"

  dfOptimalAvg <- as.data.frame(optimalAvg)
  colnames(dfOptimalAvg)[colnames(dfOptimalAvg)=="V1"] <- "Avg"

  dfOptimalComplete <- merge(merge(dfOptimalMax, dfOptimalMin, by = "noise"), dfOptimalAvg, by = "noise")

  #Plots in filled lines
  output$plot1 <- renderPlotly({
    plot_ly(dfGreedyComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = ~Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(27,158,229,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(27,158,119)'),
              name = 'Average') %>%
    add_trace(x = ~noise, y = ~dfPagerankComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = dfPagerankComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(217,95,2,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = dfPagerankComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(217,95,2)'),
              name = 'Average') %>%
    add_trace(x = ~noise, y = ~dfRegretComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = dfRegretComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(117,112,179,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = dfRegretComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(117,112,179)'),
              name = 'Average') %>%
    add_trace(x = ~noise, y = ~dfOptimalComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = dfOptimalComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(231,41,138,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = dfOptimalComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(231,41,138)'),
              name = 'Average') %>%
    layout(title = "Average, Greedy, Pagerank, Regret and Optimal Values in Algorithm",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = "Noise",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Sum of Matched Values",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE))
  })

  output$plot2 <- renderPlotly({
    plot_ly(dfPagerankComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = ~Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(0,100,80,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(0,100,80)'),
              name = 'Average') %>%
    layout(title = "Average, Greedy Values in Algorithm",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = "Noise",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Greedy Value",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE))
  })

  output$plot3 <- renderPlotly({
    plot_ly(dfRegretComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = ~Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(0,100,80,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(0,100,80)'),
              name = 'Average') %>%
    layout(title = "Average, Greedy Values in Algorithm",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = "Noise",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Greedy Value",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE))
  })

  output$plot4 <- renderPlotly({
    plot_ly(dfOptimalComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%
    add_trace(y = ~Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty', fillcolor='rgba(0,100,80,0.2)', line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%
    add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
              line = list(color='rgb(0,100,80)'),
              name = 'Average') %>%
    layout(title = "Average, Greedy Values in Algorithm",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = "Noise",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Greedy Value",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE))
  })
}

##################################
shinyApp(ui, server)
