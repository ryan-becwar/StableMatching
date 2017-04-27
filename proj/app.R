library(ggplot2)
library(plotly)
library(networkD3)
require(data.table)

# -------------------------------------------------------------------
# USER INTERFACE
# -------------------------------------------------------------------
ui <- fluidPage(
  titlePanel("Comparison of Bipartite Matching Algorithms"),

  sidebarLayout(
    sidebarPanel(
      h4("Dynamic Network Visualization"),
      p("Welcome to a this application on Bipartite Matching Algorithms. Select a data set and
        network stage to compare how the various algorithms decide which nodes should be paired.
        In this case, our graphs are networks of plant-pollinator webs."),

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
        tabPanel("Greedy Algorithm", forceNetworkOutput("greedy")),
        tabPanel("Pagerank Algorithm", forceNetworkOutput("pagerank")),
        tabPanel("Regret Algorithm", forceNetworkOutput("regret"))
      )
    )
  ),

  fluidRow(
    column(width = 8, class = "well",
      plotlyOutput("plot1", height = 680)
      ),
    column(width = 4, class = "well",
           h3("Algorithm Overview"),
           p("Run time is described with N, M being the number nodes in the left and right sides of the graph, and E being the number of edges (which is equal to N * M in most graphs)."),

            p("Optimal: O(NE) Translates the graph into a linear programming problem and solves for the optimal solution."),

             p("Regret: O(NMlogN) Determines which nodes have the steepest decrease in the value of their connections and matches these nodes with a higher priority"),

             p("Pagerank: O(NM) Represents the graph as a matrix and finds its principal eigenvector to determine the centrality of each node in the graph, then matches the most central nodes with a higher priority"),

             p("Greedy: O(NM) Nodes on one side of the graph greedily pick their best matching in a random order"),

             checkboxGroupInput("datasetGroup",
                              label = h4("Pick algorithms to view."),
                              choices = list("Optimal (pink)"=4,
                                              "Regret (purple)"=3,
                                              "Pagerank (orange)"=2,
                                               "Greedy (green)"=1),
                              selected = c(1,2,3,4))
    )
   ),

  fluidRow(
    column(width = 6, class = "well",
           plotlyOutput("bar_graph", height = 350)
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

  kbnodes <- read.csv("kb_nodes_temp.csv")
  kblinks <- read.csv("kb_links_temp.csv")

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

  greedy_pairing <- reactive({switch(input$stage, "Raw Network" = get_links()$value,
                                   "Network with Priority" = get_links()$value,
                                   "Network with Final Pairings" = get_links()$greedy_pairing)})

  pagerank_pairing <- reactive({switch(input$stage, "Raw Network" = get_links()$value,
                                     "Network with Priority" = get_links()$value,
                                     "Network with Final Pairings" = get_links()$pagerank_pairing)})

  regret_pairing <- reactive({switch(input$stage, "Raw Network" = get_links()$value,
                                     "Network with Priority" = get_links()$value,
                                     "Network with Final Pairings" = get_links()$regret_pairing)})

  output$greedy <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = greedy_group(),
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(greedy_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })

  output$pagerank <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = pagerank_group(),
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(pagerank_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })

  output$regret <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = regret_group(),
                 opacity = input$opacity, zoom=TRUE,
                 linkColour = ifelse(regret_pairing() > 0, "585C5C","F3F3F3"),
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })

  ##### BAR GRAPH STUFF #####

  x_vals <- c("regret", "greedy", "pagerank", "optimal")
  bh_vals <- c(203, 203, 196, 206)
  kb_vals <- c(125.042, 139.024, 46.083, 155.279 )
  data <- data.frame(x_vals, bh_vals, kb_vals)

  output$bar_graph <- renderPlotly({
    p <- plot_ly(data, x = ~x_vals, y = ~bh_vals, type = 'bar', name = 'BH Scores') %>%
    add_trace(y = ~kb_vals, name = 'KB Scores') %>%
    layout(title="Algorithm Performance on Real World Data",
           yaxis = list(title = 'Sum of Matched Values'),
           xaxis = list(title = ' '),
           barmode = 'group')
  })

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


  greedy_check <- reactive({is.element("1", input$datasetGroup)})
  pagerank_check <- reactive({is.element("2", input$datasetGroup)})
  regret_check <- reactive({is.element("3", input$datasetGroup)})
  optimal_check <- reactive({is.element("4", input$datasetGroup)})

  #Plots in filled lines
  output$plot1 <- renderPlotly({
    plot_ly(dfGreedyComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max', hoverinfo='none') %>%

      add_trace(y = ~Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty',
              fillcolor= ifelse(greedy_check() == TRUE, "rgba(27,158,229,0.2)","transparent"),
              line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%

      add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
              line = list(color=ifelse(greedy_check() == TRUE,'rgb(27,158,119)', 'transparent')),
              hoverinfo=ifelse(greedy_check() == TRUE, "y","none"),
              name = 'Average') %>%

      add_trace(x = ~noise, y = ~dfPagerankComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%

      add_trace(y = dfPagerankComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty',
              fillcolor=ifelse(pagerank_check() == TRUE,'rgba(217,95,2,0.2)','transparent'),
              line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%

      add_trace(x = ~noise, y = dfPagerankComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color=ifelse(pagerank_check() == TRUE,'rgb(217,95,2)', 'transparent')),
              hoverinfo=ifelse(pagerank_check() == TRUE, "y","none"),
              name = 'Average') %>%

      add_trace(x = ~noise, y = ~dfRegretComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%

      add_trace(y = dfRegretComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty',
              fillcolor=ifelse(regret_check() == TRUE,'rgba(117,112,179,0.2)', 'transparent'),
              line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%

      add_trace(x = ~noise, y = dfRegretComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color=ifelse(regret_check() == TRUE,'rgb(117,112,179)', 'transparent')),
              hoverinfo=ifelse(regret_check() == TRUE, "y","none"),
              name = 'Average') %>%

      add_trace(x = ~noise, y = ~dfOptimalComplete$Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max') %>%

      add_trace(y = dfOptimalComplete$Min, type = 'scatter', mode = 'lines',
              fill = 'tonexty',
              fillcolor= ifelse(optimal_check() == TRUE,'rgba(231,41,138,0.2)', 'transparent'),
              line = list(color = 'transparent'),
              showlegend = FALSE, name = 'Min') %>%

      add_trace(x = ~noise, y = dfOptimalComplete$Avg, type = 'scatter', mode = 'lines',
              line = list(color=ifelse(optimal_check() == TRUE,'rgb(231,41,138)', 'transparent')),
              hoverinfo=ifelse(optimal_check() == TRUE, "y","none"),
              name = 'Average') %>%

      layout(title = "Algorithm Performances on Generated Graphs",
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

        p <- plot_ly(dfPagerankComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
          line = list(color = 'transparent'),
          showlegend = FALSE, name = 'Max')
        p <- add_trace(y = ~Min, type = 'scatter', mode = 'lines',
                fill = 'tonexty', fillcolor='rgba(0,100,80,0.2)', line = list(color = 'transparent'),
                showlegend = FALSE, name = 'Min')
        p <- add_trace(x = ~noise, y = ~Avg, type = 'scatter', mode = 'lines',
                line = list(color='rgb(0,100,80)'),
                name = 'Average')

    p <- layout(title = "Average, Greedy Values in Algorithm",
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
    print(p)


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
