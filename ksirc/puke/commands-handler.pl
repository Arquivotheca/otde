sub puke_setup_ack {
  my(%ARG) = %{$_[0]};
  $PukeMSize = $ARG{'iArg'};
  print "*I* Puke: Initial Setup complete\n";
  print "*I* Puke: Communications operational\n";
}
$PUKE_DEF_HANDLER{"$PUKE_SETUP_ACK"} = \&puke_setup_ack;

sub puke_invalid_cmd {
  print "*E* Puke: Invalid command 0 ack'ed\n";
}
$PUKE_DEF_HANDLER{"$PUKE_INVALID"} = \&puke_invalid_cmd;


sub puke_widget_create_ack {
  my %ARG = %{$_[0]};

  $ARG{cArg} =~ /^(.{8,8})/;
  my $string = $1; 

  if($PUKE_CREATOR{$string}){
    &{$PUKE_CREATOR{$string}}(%ARG); # added %ARG
  }
  else {
    print "*E* Widget created: $string but no handler\n";
  }
}

$PUKE_DEF_HANDLER{"$PUKE_WIDGET_CREATE_ACK"} = \&puke_widget_create_ack;
$PUKE_DEF_HANDLER{"$PUKE_LAYOUT_NEW_ACK"} = \&puke_widget_create_ack;


# By default we ignore all the EVENT's we get sent at us.

$PUKE_DEF_HANDLER{"$PUKE_WIDGET_EVENT_NONE"} = sub {};
$PUKE_DEF_HANDLER{"$PUKE_WIDGET_EVENT_TIME"} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_MOUSEBUTTONPRESS} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_MOUSEBUTTONRELEASE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_MOUSEDBLCLICK} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_MOUSEMOVE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_KEYPRESS} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_KEYRELEASE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_KEYPRESS} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_FOCUSIN} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_FOCUSOUT} = sub {};
$PUKE_DEF_HANDLER{"$PUKE_WIDGET_EVENT_ENTER"} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_LEAVE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_PAINT} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_MOVE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_RESIZE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_CREATE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_DESTORY} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_SHOW} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_HIDE} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_EVENT_CLOSE} = sub {};

$PUKE_DEF_HANDLER{$PUKE_WIDGET_RESIZE_ACK} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_REPAINT_ACK} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_MOVE_ACK} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_SHOW_ACK} = sub {};
$PUKE_DEF_HANDLER{$PUKE_WIDGET_HIDE_ACK} = sub {};

$PUKE_DEF_HANDLER{$PUKE_LINED_SET_TEXT_ACK} = sub {};
1;
