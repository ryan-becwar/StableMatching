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

      radioButtons("algorithim", label = h4("Choose an alogorithim."),
                   choices = list("Algorithim1" = 1, "Algorithim2" = 2,
                                  "Algorithim3" = 3),selected = 1)
    ),
    mainPanel(
      tabsetPanel(
        tabPanel("Raw Network", forceNetworkOutput("force1")),
        tabPanel("Network with Priority"),
        tabPanel("Final Pairing")
      )
    )
  ),

  fluidRow(
    column(width = 4, class = "well",
      h4("Plotly Example"),
      plotlyOutput("plot1", height = 300)
      ),

    column(width = 8, class = "well",
      h4("Another Graph"),
      plotlyOutput("plot2", height = 300)
    ),

  column(width = 8, class = "well",
    h4("Another Graph"),
    plotlyOutput("plot3", height = 300)
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
# SERVER PORTION
# -------------------------------------------------------------------
server <- function(input, output) {

  #-- Load Network Visualization Dataframes --#
  bhnodes <- read.csv("bh1987_nodes.csv")
  bhlinks <- read.csv("bh1987_links.csv")

  kbnodes <- read.csv("kb2009_nodes.csv")
  kblinks <- read.csv("kb2009_links.csv")

  all_nodes <- list(kbnodes, bhnodes)
  all_links <- list(kblinks, bhlinks)

  #-- Get Dataset for Network from User Input --#
  get_nodes <- reactive({switch(input$dataset,"KB2009" = kbnodes,"BH1987" = bhnodes)})
  get_links <- reactive({switch(input$dataset,"KB2009" = kblinks,"BH1987" = bhlinks)})


  output$force1 <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Nodesize = "degree", Group = "group", opacity = input$opacity, zoom=TRUE,
                 fontSize = 16, colourScale = JS("d3.scaleOrdinal(d3.schemeCategory10);"),
                 legend = TRUE)
  })


  output$force2 <- renderForceNetwork({
    forceNetwork(Links = get_links(), Nodes = get_nodes(), Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Group = "origin", Nodesize = "degree", opacity = input$opacity,
                 fontSize = 16, zoom=TRUE)
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
  data = read.csv("value_data.csv")
  df = data.frame(noise= data$noise,
                  greedy=data$greedy_mean,
                  pagerank=data$pagerank_value,
                  optimal=data$optimal_value)
  dfReduced = data.frame(noise= data$noise,
             greedy= data$greedy_mean)
  dt <- data.table(dfReduced)

  #At each noise value, take only the max, min and mean value
  dtMax = dt[ , max(greedy), by = noise]
  dtMin = dt[ , min(greedy), by = noise]
  dtAvg = dt[ , mean(greedy), by = noise]


  #The above functions rename greedy to V1, so they must be renamed
  dfMax <- as.data.frame(dtMax)
  colnames(dfMax)[colnames(dfMax)=="V1"] <- "Max"

  dfMin <- as.data.frame(dtMin)
  colnames(dfMin)[colnames(dfMin)=="V1"] <- "Min"

  dfAvg <- as.data.frame(dtAvg)
  colnames(dfAvg)[colnames(dfAvg)=="V1"] <- "Avg"

  dfComplete <- merge(merge(dfMax, dfMin, by = "noise"), dfAvg, by = "noise")

  #Plots in filled lines
  output$plot1 <- renderPlotly({
    plot_ly(dfComplete, x = ~noise, y = ~Max, type = 'scatter', mode = 'lines',
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

  output$plot2 <- renderPlotly({
    plot_ly(as.data.frame(dtMin), x = ~noise, y = ~V1, type="scatter", mode="lines")
  })

  output$plot3 <- renderPlotly({
    plot_ly(as.data.frame(dtAvg), x = ~noise, y = ~V1, type="scatter", mode="lines")
  })
}

shinyApp(ui, server)
