library(ggplot2)
library(plotly)
library(networkD3)

# -------------------------------------------------------------------
# USER INTERFACE
# -------------------------------------------------------------------
ui <- fluidPage(
  titlePanel("Data Science Project 2017"),

  sidebarLayout(
    sidebarPanel(
      sliderInput("opacity", "Opacity", 0.8, min = 0.1,
                    max = 1, step = .1)
    ),
    mainPanel(
      tabsetPanel(
        tabPanel("Simple Network", simpleNetworkOutput("force2")),
        tabPanel("Force Network", forceNetworkOutput("force1"))
      )
    )
  ),

  fluidRow(
    column(width = 4, class = "well",
      h4("Plotly Example"),
      plotlyOutput("plot1", height = 300)
      ),

    column(width = 4, class = "well",
      h4("Another Graph"),
      plotlyOutput("plot2", height = 300)
    ),

    column(width = 4, class = "well",
       h4("A Third Graph"),
       plotlyOutput("plot3", height = 300)
    )
   )
)

# -------------------------------------------------------------------
# SERVER PORTION
# -------------------------------------------------------------------
server <- function(input, output) {

  ##### NETWORK GRAPHS #####
  bhnodes <- read.csv("bh1987_nodes.csv")
  bhlinks <- read.csv("bh1987_links.csv")
  
  output$force1 <- renderForceNetwork({
    forceNetwork(Links = bhlinks, Nodes = bhnodes, Source = "source",
                 Target = "target", Value = "value", NodeID = "name",
                 Group = "genus", Nodesize = "degree", opacity = input$opacity,
                 fontSize = 16)
  })
  
  kbnodes <- read.csv("kb2009_nodes.csv")
  kblinks <- read.csv("kb2009_links.csv")
  
  output$force2 <- renderForceNetwork({
    forceNetwork(Links = kblinks, Nodes = kbnodes, Source = "source",
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

  data = read.csv("value_data.csv")
  df = data.frame(noise= data$noise, 
                  greedy=data$greedy_mean, 
                  pagerank=data$pagerank_value, 
                  optimal=data$optimal_value) 
  
  output$plot1 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~greedy, type="scatter", mode="lines")
  })
  
  output$plot2 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~optimal, type="scatter", mode="lines")
  })

  output$plot3 <- renderPlotly({
    plot_ly(df, x = ~noise, y = ~pagerank, type="scatter", mode="lines")
  })

  # When a double-click happens, check if there's a brush on the plot.
  # If so, zoom to the brush bounds; if not, reset the zoom.
  observe({
    brush <- input$plot2_brush
    if (!is.null(brush)) {
      ranges2$x <- c(brush$xmin, brush$xmax)
      ranges2$y <- c(brush$ymin, brush$ymax)

    } else {
      ranges2$x <- NULL
      ranges2$y <- NULL
    }
  })

}

shinyApp(ui, server)
