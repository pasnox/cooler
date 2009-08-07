#include "UIPadConfiguration.h"

#include <QDebug>

UIPadConfiguration::UIPadConfiguration( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	
	connect( psConfiguration, SIGNAL( aboutToChangeStrokeKey( Globals::PadStroke, int, bool& ) ), this, SLOT( padSettings_aboutToChangeStrokeKey( Globals::PadStroke, int, bool& ) ) );
	connect( psConfiguration, SIGNAL( aboutToChangeActionKey( Globals::PadAction, int, bool& ) ), this, SLOT( padSettings_aboutToChangeActionKey( Globals::PadAction, int, bool& ) ) );
	connect( psConfiguration, SIGNAL( strokeKeyChanged( Globals::PadStroke, int ) ), this, SLOT( padSettings_strokeKeyChanged( Globals::PadStroke, int ) ) );
	connect( psConfiguration, SIGNAL( actionKeyChanged( Globals::PadAction, int ) ), this, SLOT( padSettings_actionKeyChanged( Globals::PadAction, int ) ) );
	connect( psConfiguration, SIGNAL( editingFinished() ), this, SLOT( padSettings_editingFinished() ) );
}

UIPadConfiguration::~UIPadConfiguration()
{
}

void UIPadConfiguration::setPads( const PadSettingsList& pads, int maxPad )
{
	cbPads->clear();
	
	mPads = pads;
	mMaxPad = maxPad;
	
	while ( mPads.count() > mMaxPad )
	{
		mPads.removeLast();
	}
	
	while ( mPads.count() < mMaxPad )
	{
		mPads << PadSettings();
	}
	
	for ( int i = 0; i < mMaxPad; i++ )
	{
		cbPads->addItem( trUtf8( "Player N° %1" ).arg( i +1 ) );
	}
}

int UIPadConfiguration::currentPadIndex() const
{
	return cbPads->currentIndex();
}

void UIPadConfiguration::setCurrentPadIndex( int index )
{
	cbPads->setCurrentIndex( index );
}

void UIPadConfiguration::on_cbPads_currentIndexChanged( int index )
{
	psConfiguration->setPad( mPads.value( index ) );
}

void UIPadConfiguration::on_pbConfigure_clicked()
{
	cbPads->setEnabled( false );
	pbConfigure->setEnabled( false );
	psConfiguration->setup();
}

void UIPadConfiguration::padSettings_aboutToChangeStrokeKey( Globals::PadStroke stroke, int key, bool& accept )
{
	for ( int i = 0; i < mPads.count(); i++ )
	{
		const PadSettings& pad = mPads.at( i );
		
		if ( pad.isKeyUsed( key ) )
		{
			accept = false;
			
			if ( currentPadIndex() == i )
			{
				if ( pad.keyStroke( key ) == stroke )
				{
					accept = true;
				}
			}
			
			return;
		}
	}
}

void UIPadConfiguration::padSettings_aboutToChangeActionKey( Globals::PadAction action, int key, bool& accept )
{
	for ( int i = 0; i < mPads.count(); i++ )
	{
		const PadSettings& pad = mPads.at( i );
		
		if ( pad.isKeyUsed( key ) )
		{
			accept = false;
			
			if ( currentPadIndex() == i )
			{
				if ( pad.keyAction( key ) == action )
				{
					accept = true;
				}
			}
			
			return;
		}
	}
}

void UIPadConfiguration::padSettings_strokeKeyChanged( Globals::PadStroke stroke, int key )
{
	mPads[ currentPadIndex() ].setStrokeKey( stroke, key );
}

void UIPadConfiguration::padSettings_actionKeyChanged( Globals::PadAction action, int key )
{
	mPads[ currentPadIndex() ].setActionKey( action, key );
}

void UIPadConfiguration::padSettings_editingFinished()
{
	cbPads->setEnabled( true );
	pbConfigure->setEnabled( true );
}
